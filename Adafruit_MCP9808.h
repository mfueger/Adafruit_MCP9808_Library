/**************************************************************************/
/*!
    @file     Adafruit_MCP9808.h
    @author   K. Townsend (Adafruit Industries)
    @license  BSD (see license.txt)

	This is a library for the Adafruit MCP9808 Temp Sensor breakout board
	----> http://www.adafruit.com/products/1782

	Adafruit invests time and resources providing this open source code,
	please support Adafruit and open-source hardware by purchasing
	products from Adafruit!

	@section  HISTORY

    v1.0  - First release
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

#define MCP9808_I2CADDR_DEFAULT        0x18

// Configuration Register Bits (two bytes total)
// bit 15-11 unimplemented
// bit 10-9  Tupper and Tlower Limit Hysteresis Bits
#define MCP9808_REG_CONFIG_SHUTDOWN    0x0100 // bit 8
#define MCP9808_REG_CONFIG_CRITLOCKED  0x0080 // bit 7
#define MCP9808_REG_CONFIG_WINLOCKED   0x0040 // bit 6
#define MCP9808_REG_CONFIG_INTCLR      0x0020 // bit 5
#define MCP9808_REG_CONFIG_ALERTSTAT   0x0010 // bit 4
#define MCP9808_REG_CONFIG_ALERTCTRL   0x0008 // bit 3
#define MCP9808_REG_CONFIG_ALERTSEL    0x0004 // bit 2
#define MCP9808_REG_CONFIG_ALERTPOL    0x0002 // bit 1
#define MCP9808_REG_CONFIG_ALERTMODE   0x0001 // bit 0

// Register Pointer Bits
#define MCP9808_REG_CONFIG             0x01
#define MCP9808_REG_UPPER_TEMP         0x02
#define MCP9808_REG_LOWER_TEMP         0x03
#define MCP9808_REG_CRIT_TEMP          0x04
#define MCP9808_REG_AMBIENT_TEMP       0x05
#define MCP9808_REG_MANUF_ID           0x06
#define MCP9808_REG_DEVICE_ID          0x07
#define MCP9808_REG_RESOLUTION         0x08

class Adafruit_MCP9808 {

  public:

    Adafruit_MCP9808();

    boolean begin(uint8_t a = MCP9808_I2CADDR_DEFAULT);

    float readTempC( void );
    float readTempF( void ); // THIS HAS NOT BEEN DECLARED!

    int shutdown_wake( uint8_t sw_ID );

    uint16_t read16(uint8_t reg);
    void write16(uint8_t reg, uint16_t val);

  private:

    uint8_t _i2caddr;
};
