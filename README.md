BME68X Environmental Sensor from seengreat:www.seengreat.com
 =======================================
# Ⅰ Directions
![image](https://github.com/seengreat/BME68X-Environmental-Sensor/blob/main/1.jpg)<br>
## 1.1 Product Overview
This product is a high-precision environmental sensor featuring the BME68X sensor, which enables monitoring of temperature, humidity, atmospheric pressure, and VOC gases. The chip's temperature measurement range is -40 to +85°C, humidity range is 0–100%, and atmospheric pressure range is 300–1100 hPa.<br>
The module connects to the development board via a PH2.0 6-pin cable for environmental monitoring. The board supports SPI/I2C interfaces, and we provide example programs for Arduino, Raspberry Pi, Raspberry Pi Pico, and ESP32 platforms. These example programs allow real-time monitoring of temperature, humidity, atmospheric pressure, and gases.<br>

## 1.2 Product Specifications
|Sensor Chip|BME680|BME688|
|-------------|---------|---------|
|Dimensions|30mm (L) × 20mm (W)| |
|Signal Interface|I2C/SPI| |
|Supply Voltage|3.3V/5V| |
|Temperature Range|-40 to +85℃| |
|Temperature Accuracy| ±1℃ (0–65℃)|±0.5℃ (0–65℃)|
|Humidity Measurement|0–100%RH (Resolution: 0.008%RH, Accuracy: ±3%RH)| |
|Pressure Range|300–1100 hPa| |
|Pressure Accuracy|±0.6hPa (0–65℃)| |
|Gas Detection|VOC gas variation detection (Bosch software required for IAQ calculation)|VOC and VSC gas variation detection (Bosch software required for IAQ calculation and integrated AI functions)|
<br>
## 1.3 Dimensions
![image](https://github.com/seengreat/BME68X-Environmental-Sensor/blob/main/1.jpg)<br>
# Ⅱ Usage
The onboard I2C address setting slide switch is by default in the 0x77 position. When using SPI communication, ensure the switch is also set to 0x77.<br>
Note: In the example programs for all platforms, the gas detection result is represented as a resistance value. To convert this to the IAQ (Indoor Air Quality) index, you must use the official BSEC software library (non-open-source). Bosch imposes certain restrictions and licensing on the use of this software library. Users should review the documentation and terms before integrating it. The link to the BSEC software library is: https://www.bosch-sensortec.com/software-tools/software/previous-bsec-software-versions/<br>

## 2.1 Resources Overview
![image](https://github.com/seengreat/BME68X-Environmental-Sensor/blob/main/1.jpg)<br>
1.PH2.0 6-PIN Connector: Used for connecting the sensor module to the development board.<br>
2.RT9193-33 Linear Regulator: Provides a stable power supply with low power consumption and quick startup time.<br>
3.BME68X Sensor: The core component for measuring temperature, humidity, pressure, and gas levels.<br>
4.I2C Interface Pads: Solder pads for I2C communication interface.<br>
5.Slide Switch for I2C Address Configuration: Used to set the I2C address for communication.<br>
## 2.2 Interface Definition
|Pin Function|I2C|SPI|
|--------------|---|----|
|VCC|Power positive (3.3V/5V)|Power positive (3.3V/5V)|
|GND|Power ground|Power ground|
|SCL/SCK|Clock line|Serial clock|
|SDA/MOSI|Data line|Master Output/Slave Input
|MISO/ADDR|Not connected (NC)|Master Input/Slave Output|
|CS|Not connected (NC)|Chip select signal|
<br>
Table 2-1: BME68X Environmental Sensor Pin Definition<br>
## 2.3 Usage
For more details about Usage<br>
https://seengreat.com/wiki/162/bme680-environmental-sensor-bme688-environmental-sensor<br>

__Thank you for choosing the products of Shengui Technology Co.,Ltd. For more details about this product, please visit:
www.seengreat.com__
