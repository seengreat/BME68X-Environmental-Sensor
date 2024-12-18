/**
 * Copyright (C) 2023 Bosch Sensortec GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>

#include "bme68x.h"
#include "common.h"
#include "sg_lgpio.h"
/*****************************************************************************
 * For the selection of the communication interface,
 * see the definition of USE_IIC in the bme68x_defs.h file.
 * use I2C or SPI communication mode, 0 for SPI, 1 for I2C
 * #define USE_IIC   0
 * if use spi mode,Pins wiring in wiringpi number as:
 * Board       Raspberry Pi
 *  GND ---------> GND
 *  VCC ---------> 3.3V
 *  CS  ---------> D25
 * MOSI/SDA------> MOSI
 * SCK/SCL ------> SCK
 * MISO----------> MISO
 * 
 * **************************************************************************/
/*****************************************************************************
 * if use iic mode,Pins wiring in wiringpi number as:
 * Board       Raspberry Pi
 *  GND---------> GND
 *  VCC---------> 3.3V
 *  CS ---------> NC
 * MOSI/SDA-----> SDA1
 * SCK/SCL -----> SCL1
 * MISO---------> NC
 * 
 * **************************************************************************/
/****************************************************************************
 * https://seengreat.com
**********************************************************************/
/*                         Test code                                   */
/***********************************************************************/

int main(void)
{
    struct bme68x_dev bme;
    int8_t rslt;
    struct bme68x_conf conf;
    struct bme68x_heatr_conf heatr_conf;
    struct bme68x_data data;
    uint32_t del_period;
    uint32_t time_ms = 0;
    uint8_t n_fields;
    uint16_t sample_count = 1;

    /* Interface preference is updated as a parameter
     * For I2C : BME68X_I2C_INTF
     * For SPI : BME68X_SPI_INTF
     */
    rslt = bme68x_interface_init(&bme, BME68X_SPI_INTF);
    bme68x_check_rslt("bme68x_interface_init", rslt);

    rslt = bme68x_init(&bme);
    bme68x_check_rslt("bme68x_init", rslt);

    /* Check if rslt == BME68X_OK, report or handle if otherwise */
    conf.filter = BME68X_FILTER_OFF;
    conf.odr = BME68X_ODR_NONE;
    conf.os_hum = BME68X_OS_16X;
    conf.os_pres = BME68X_OS_1X;
    conf.os_temp = BME68X_OS_2X;
    rslt = bme68x_set_conf(&conf, &bme);
    printf("set conf OK\r\n");
    bme68x_check_rslt("bme68x_set_conf", rslt);

    /* Check if rslt == BME68X_OK, report or handle if otherwise */
    heatr_conf.enable = BME68X_ENABLE;
    heatr_conf.heatr_temp = 300;
    heatr_conf.heatr_dur = 100;
    rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme);
    bme68x_check_rslt("bme68x_set_heatr_conf", rslt);

    printf("Sample, TimeStamp(ms), Temperature(deg C), Pressure(Pa), Humidity(%%), Gas resistance(ohm), Status\n");

    while (1)
    {
        rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
        bme68x_check_rslt("bme68x_set_op_mode", rslt);

        /* Calculate delay period in microseconds */
        del_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + (heatr_conf.heatr_dur * 1000);
        bme.delay_us(del_period, bme.intf_ptr);

        /* Check if rslt == BME68X_OK, report or handle if otherwise */
        rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme);
        bme68x_check_rslt("bme68x_get_data", rslt);
        delay(700);
        if (n_fields)
        {
#ifdef BME68X_USE_FPU
            printf("%u, %.2f, %.2f, %.2f, %.2f, 0x%x\n",
                   sample_count,
                   data.temperature,
                   data.pressure,
                   data.humidity,
                   data.gas_resistance,
                   data.status);
#else
            printf("%u, %d, %lu, %lu, %lu, 0x%x\n",
                   sample_count,
                   (data.temperature / 100),
                   (long unsigned int)data.pressure,
                   (long unsigned int)(data.humidity / 1000),
                   (long unsigned int)data.gas_resistance,
                   data.status);
#endif
            sample_count++;
        }
    }

    return rslt;
}
