// I2C library abstracts basic R/W functions

#ifndef I2CLIB_H
#define I2CLIB_H

#include "Arduino.h"
#include "Wire.h"

class I2Clib
{
public:
	I2Clib();

	static int8_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, uint8_t *data);
	static int8_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
	static int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
	static int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);

	static bool writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, uint8_t data);
	static bool writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
	static bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
	static bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data);

};

#endif