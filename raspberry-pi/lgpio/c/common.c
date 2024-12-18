/**
 * Copyright (C) 2023 Bosch Sensortec GmbH. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "bme68x.h"
#include "bme68x_defs.h"
#include "common.h"

#include <unistd.h>
#include "sg_lgpio.h"

#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

/******************************************************************************/
/*!                 Macro definitions                                         */
/******************************************************************************/
/*!                         Own macro definition                              */
//Raspberry 4B platform's default I2C device file
#define IIC_DEV  "/dev/i2c-1"
#define SPI_CH   0
#define PIN_CS   25
/******************************************************************************/
/*!                Static variable definition                                 */
struct identifier id;

/******************************************************************************/
/*!                User interface functions                                   */

/*!
 * I2C read function map to COINES platform
 */
BME68X_INTF_RET_TYPE bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    write(id.fd, &reg_addr, 1);
    read(id.fd, reg_data, (uint16_t)len);
    return 0;
}
/*!
 * I2C write function map to COINES platform
 */
BME68X_INTF_RET_TYPE bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t *buf;

    buf = malloc(len + 1);
    buf[0] = reg_addr;
    memcpy(buf + 1, reg_data, len);
    if (write(id.fd, buf, len + 1) < (uint16_t)len)
    {
        return BME68X_E_COM_FAIL;
    }
    free(buf);

    return BME68X_OK;
}

/*!
 * SPI read function map to COINES platform
 */
BME68X_INTF_RET_TYPE bme68x_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    Lgpio_write_pin(PIN_CS,LGPIO_HIGH);
    Lgpio_write_pin(PIN_CS,LGPIO_LOW); 
    LG_SPI_writebyte(reg_addr);
    LG_SPI_readbytes(reg_data, (uint16_t)len);
    Lgpio_write_pin(PIN_CS,LGPIO_HIGH);
    return 0;
}

/*!
 * SPI write function map to COINES platform
 */
BME68X_INTF_RET_TYPE bme68x_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t *buf;
    Lgpio_write_pin(PIN_CS,LGPIO_HIGH);
    Lgpio_write_pin(PIN_CS,LGPIO_LOW); 
    LG_SPI_writebyte(reg_addr);
    LG_SPI_write_bytes(reg_data, (uint16_t)len);
    Lgpio_write_pin(PIN_CS,LGPIO_HIGH);

    return BME68X_OK;
}

/*!
 * Delay function map to COINES platform
 */
void bme68x_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;
    usleep(period);
}

void bme68x_check_rslt(const char api_name[], int8_t rslt)
{
    switch (rslt)
    {
        case BME68X_OK:
            /* Do nothing */
            break;
        case BME68X_E_NULL_PTR:
            printf("API name [%s]  Error [%d] : Null pointer\r\n", api_name, rslt);
            break;
        case BME68X_E_COM_FAIL:
            printf("API name [%s]  Error [%d] : Communication failure\r\n", api_name, rslt);
            break;
        case BME68X_E_INVALID_LENGTH:
            printf("API name [%s]  Error [%d] : Incorrect length parameter\r\n", api_name, rslt);
            break;
        case BME68X_E_DEV_NOT_FOUND:
            printf("API name [%s]  Error [%d] : Device not found\r\n", api_name, rslt);
            break;
        case BME68X_E_SELF_TEST:
            printf("API name [%s]  Error [%d] : Self test error\r\n", api_name, rslt);
            break;
        case BME68X_W_NO_NEW_DATA:
            printf("API name [%s]  Warning [%d] : No new data found\r\n", api_name, rslt);
            break;
        default:
            printf("API name [%s]  Error [%d] : Unknown error code\r\n", api_name, rslt);
            break;
    }
}

int8_t bme68x_interface_init(struct bme68x_dev *dev, uint8_t intf)
{
    int8_t rslt = BME68X_OK;
    uint8_t r_buf[2] = {0,0};
    uint8_t w_buf[2] = {0xd0,0};
#if USE_IIC
    if ((id.fd = open(IIC_DEV, O_RDWR)) < 0)
    {
        fprintf(stderr, "Failed to open the i2c bus %s\n", IIC_DEV);
        exit(1);
    }
    else
    {
        printf("open i2c dev success:%d\r\n",id.fd);
    }

    rslt = ioctl(id.fd, I2C_SLAVE, BME68X_I2C_ADDR_LOW);
    if( rslt < 0)
    {
        fprintf(stderr, "Failed to acquire bus access and/or talk to slave 0x76.\n");
        exit(1);
    }
    else
    {
        write(id.fd, w_buf, 1);
        read(id.fd, r_buf, 1);
        if(r_buf[0]==0x61)
        {
           id.dev_addr = BME68X_I2C_ADDR_LOW;
           printf("ioctl i2c low dev success:%02x\r\n",BME68X_I2C_ADDR_LOW);
        }
        else
        {
            rslt = ioctl(id.fd, I2C_SLAVE, BME68X_I2C_ADDR_HIGH);
            if( rslt < 0)
            {
                fprintf(stderr, "Failed to acquire bus access and/or talk to slave 0x77.\n");
                exit(1);
            }
            else
            {
                write(id.fd, w_buf, 1);
                read(id.fd, r_buf, 1);
                if(r_buf[0]==0x61)
                {
                    id.dev_addr = BME68X_I2C_ADDR_HIGH;
                    printf("ioctl i2c high dev success:%02x\r\n",BME68X_I2C_ADDR_HIGH);
                }
                else
                {
                    fprintf(stderr, "Unable to identify BME680.\n");
                    close(id.fd);
                    exit(1);                    
                }               
            }
        }
    }
    /* Make sure to select BME68X_I2C_ADDR_LOW or BME68X_I2C_ADDR_HIGH as needed */
    
    dev->intf = BME68X_I2C_INTF;
    dev->read = bme68x_i2c_read;
    dev->write = bme68x_i2c_write;
    dev->delay_us = bme68x_delay_us;
    printf("using i2c interface\r\n");  
#else 
    Lgpio_export(); 
    Lgpio_set_pinmode(PIN_CS,LGPIO_OUTPUT);
    LG_SPI_Init(0, 0, 1000000);
    Lgpio_write_pin(PIN_CS,LGPIO_LOW); //select the SPI interface
    id.dev_addr = 0;
    id.fd = 0;

    dev->intf = BME68X_SPI_INTF;
    dev->read = bme68x_spi_read;
    dev->write = bme68x_spi_write;
    dev->delay_us = bme68x_delay_us; 
    printf("using spi interface\r\n");   
#endif
    /* Update interface pointer with the structure that contains both device address and file descriptor */
    //dev->intf_ptr = &id;

    /* Initialize the BME68X */
    rslt = bme68x_init(dev);
    if (rslt != BME68X_OK)
    {
        fprintf(stderr, "Failed to initialize the device (code %+d).\n", rslt);
        exit(1);
    }

    dev->amb_temp = 25; //The ambient temperature in deg C is used for defining the heater temperature 

    return 0;
}

