/**************************************************************************/
/*!
    @file     Adafruit_MCP9808.cpp
    @author   K.Townsend (Adafruit Industries)
	@license  BSD (see license.txt)

	I2C Driver for Microchip's MCP9808 I2C Temp sensor

	This is a library for the Adafruit MCP9808 breakout
	----> http://www.adafruit.com/products/1782

	Adafruit invests time and resources providing this open source code,
	please support Adafruit and open-source hardware by purchasing
	products from Adafruit!

	@section  HISTORY

    v1.0 - First release
*/
/**************************************************************************/
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
  #include "TinyWireM.h"
  #define Wire TinyWireM
#else
  #include <Wire.h>
#endif

#include "Adafruit_MCP9808.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new MCP9808 class
*/
/**************************************************************************/
Adafruit_MCP9808::Adafruit_MCP9808() {
}

/**************************************************************************/
/*!
    @brief  Setups the HW
*/
/**************************************************************************/
boolean Adafruit_MCP9808::begin(uint8_t addr) {
  _i2caddr = addr;
  Wire.begin();

  if (read16(MCP9808_REG_MANUF_ID) != 0x0054) return false;
  if (read16(MCP9808_REG_DEVICE_ID) != 0x0400) return false;

  return true;
}

/**************************************************************************/
/*!
    @brief  Reads the 16-bit temperature register and returns the Centigrade
            temperature as a float.

*/
/**************************************************************************/
float Adafruit_MCP9808::readTempC( void ) {
  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

  float temp = t & 0x0FFF; // clear flag bits; bits 0-11 - ambient temperature in two's complement
  temp /=  16.0;
  if (t & 0x1000) temp -= 256; // bit 12 is the sign bit - 0 = Ta >= 0; 1 = Ta < 0

  return temp;
}

/**************************************************************************/
/*!
    @brief  Reads the 16-bit temperature register and returns the Fahrenheit
            temperature as a float.

*/
/**************************************************************************/
float Adafruit_MCP9808::readTempF( void ) {
  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

  float temp = t & 0x0FFF; // clear flag bits; bits 0-11 - ambient temperature in two's complement
  temp /=  16.0;
  if (t & 0x1000) temp -= 256; // bit 12 is the sign bit - 0 = Ta >= 0; 1 = Ta < 0
  temp = temp * 1.8 + 32;

  return temp;
}

//*************************************************************************
// Set Sensor to Shutdown-State or wake up (Conf_Register BIT8)
// 1= shutdown / 0= wake up
//*************************************************************************

int Adafruit_MCP9808::shutdown_wake( uint8_t sw_ID ) {
  uint16_t conf_shutdown;
  uint16_t conf_register = read16(MCP9808_REG_CONFIG); // read the entire config register (two bytes)

  if (sw_ID == 1) {
    conf_shutdown = conf_register | MCP9808_REG_CONFIG_SHUTDOWN; // OR to set the bit
    write16(MCP9808_REG_CONFIG, conf_shutdown);
  }

  if (sw_ID == 0) {
    conf_shutdown = conf_register ^ MCP9808_REG_CONFIG_SHUTDOWN; // XOR to clear the bit
    write16(MCP9808_REG_CONFIG, conf_shutdown);
  }

  return 0;
}

/**************************************************************************/
/*!
    @brief  Low level 16 bit read and write procedures!
*/
/**************************************************************************/

uint16_t Adafruit_MCP9808::read16(uint8_t reg) {
  uint16_t val;

  Wire.beginTransmission(_i2caddr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)2);
  val = Wire.read(); // read upper byte
  val <<= 8; // left shift by 8 places
  val |= Wire.read(); // bitwise or equal, leaves bits set

  return val;
}

void Adafruit_MCP9808::write16(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(_i2caddr);
  Wire.write((uint8_t)reg);
  Wire.write(value >> 8);
  Wire.write(value & 0xFF);
  Wire.endTransmission();
}
