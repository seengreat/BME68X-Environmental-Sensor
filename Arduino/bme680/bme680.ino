#include <dummy_rp2040.h>


/**
 * Copyright (C) 2021 Bosch Sensortec GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 */

#include "Arduino.h"
#include "bme68xLibrary.h"

//This demo run in  forced_mode
//If USEIIC is 1 means use I2C interface, If it is 0,use SPI interface
#define USE_IIC 1
/*** connected for Arduino UNO **
 *           SPI        I2C
 * VCC     5V/3.3V    5V/3.3V
 * GND       GND        GND
 * SCK       D13        SCL
 * MOSI      D11        SDA
 * MISO      D12       NC/GND   //when set the I2C address to 0x77,this pin is NC,
 *                                when set the I2C address to 0x76,this pin is NC or GND
 * CS        D10        NC
 */
 /*** connected for Arduino Mega **
 *           SPI        I2C
 * VCC     5V/3.3V    5V/3.3V
 * GND       GND        GND
 * SCK       D52        SCL1
 * MOSI      D51        SDA1
 * MISO      D50       NC/GND   //when set the I2C address to 0x77,this pin is NC,
 *                                when set the I2C address to 0x76,this pin is NC or GND
 * CS        D53        NC
 */
#if USE_IIC
#define ADDR_I2C  0x77 // I2C
#else
#define PIN_CS SS
#endif

Bme68x bme;

/**
 * @brief Initializes the sensor and hardware settings
 */
void setup(void)
{
  Serial.begin(115200);
  
  while (!Serial)
    delay(10);
#if USE_IIC
  Wire.begin(); 
#else
  SPI.begin();
#endif
    
  /* initializes the sensor based on SPI library */
#if USE_IIC
  Serial.print("use I2C\n");
  bme.begin(ADDR_I2C, Wire);
#else
  Serial.print("use SPI\n");
  bme.begin(PIN_CS, SPI);
#endif
  Serial.print("run in forced_mode\n");
  if(bme.checkStatus())
  {
    if (bme.checkStatus() == BME68X_ERROR)
    {
      Serial.println("Sensor error:" + bme.statusString());
      return;
    }
    else if (bme.checkStatus() == BME68X_WARNING)
    {
      Serial.println("Sensor Warning:" + bme.statusString());
    }
  }
  
  /* Set the default configuration for temperature, pressure and humidity */
  bme.setTPH();

  /* Set the heater configuration to 300 deg C for 100ms for Forced mode */
  bme.setHeaterProf(300, 100);

  Serial.println("TimeStamp(ms), Temperature(deg C), Pressure(Pa), Humidity(%), Gas resistance(ohm), Status");
}

void loop(void)
{
  bme68xData data;

  bme.setOpMode(BME68X_FORCED_MODE);
  delay(785+bme.getMeasDur()/200);

  if (bme.fetchData())
  {
    bme.getData(data);
    Serial.print(String(millis()) + ", ");
    Serial.print(String(data.temperature) + ", ");
    Serial.print(String(data.pressure) + ", ");
    Serial.print(String(data.humidity) + ", ");
    Serial.print(String(data.gas_resistance) + ", ");
    Serial.println(data.status, HEX);
  }
}
