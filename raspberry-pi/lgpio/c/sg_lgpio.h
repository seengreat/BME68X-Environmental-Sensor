
#ifndef _SG_LGPIO_H_
#define _SG_LGPIO_H_

#include <stdint.h>
#include <stdio.h>
#include <lgpio.h>

#define LGPIO_INPUT  0
#define LGPIO_OUTPUT 1

#define LGPIO_LOW  0
#define LGPIO_HIGH 1

extern int UART_handle;
extern int I2C_handle;
extern int SPI_handle;
extern int Gpio_handle;

void delay(uint32_t xms);
int Lgpio_export(void);
int Lgpio_set_pinmode(int pin_number,int mode);
int Lgpio_read_pin(int pin_number);
int Lgpio_write_pin(int Pin, int value);

void LG_I2C_Init(uint8_t dev_addr);
void LG_UART_Init(const char *dev, int baud);
void LG_UART_writebyte(uint8_t value);
int LG_UART_readbyte(void);
void LG_UART_writebytes(const char *txbuf,int len);
void LG_UART_readbytes(char *rxbuf,int len);
void LG_SPI_Init(int dev, int chain, int baud);
void LG_SPI_writebyte(uint8_t value);
char LG_SPI_readbyte(void);
void LG_SPI_readbytes(char *rbuf, uint16_t len);
void LG_SPI_write_bytes(const uint8_t *data, uint32_t len);
void LG_PWM(int pin_number, float freq, float duty);

#endif
