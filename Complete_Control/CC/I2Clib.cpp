// I2C library abstracts basic R/W functions

#include "I2Clib.h"

/** Default constructor.
*/
I2Clib::I2Clib()
{
}

/** Read a single bit from an 8-bit device register.
 * @param  devAddr I2C slave device address
 * @param  regAddr register to read from
 * @param  bitPos  bit position to read from (0-7)
 * @param  data    container for single bit value
 * @return         Status of read operation (number of bytes read)
*/
int8_t I2Clib::readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, uint8_t *data)
{
    uint8_t b;
    uint8_t status = readByte(devAddr, regAddr, &b);
    *data = b & (1 << bitPos);
    return status;
}

/** Read multiple bits from an 8-bit device register.
 * @param devAddr  I2C slave device address
 * @param regAddr  register to read from
 * @param bitStart first position to read from (0-7)
 * @param length   number of bits to read
 * @param data     container for multiple bits
 * @return         Status of read operation (number of bytes read)
 */
int8_t I2Clib::readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data)
// 01000101  byte to read
// 76543210  bit positions
//    xxx    bitStart: 4, length: 3
//    001    read
//      001  output
{
    uint8_t status, temp_data, temp = 0;
    if ((status = readByte(devAddr, regAddr, &temp_data)) != 0)
    {
        for (uint8_t i = bitStart; i > bitStart - length; i--)
        {
            temp |= (temp_data & (1 << i));
        }
        temp >>= (bitStart - length + 1);
        *data = temp;
    }
    return status;
}

/** Read a single byte from an 8-bit device register
 * @param devAddr I2C slave device address
 * @param regAddr register to read from
 * @param data    container for byte value read from device
 * @return        status of read operation (number of bytes read)
 */
 int8_t I2Clib::readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data)
 {
    return readBytes(devAddr, regAddr, 1, data);
 }

/** Read bytes from consecutive 8-bit device registers
 * @param devAddr I2C slave device address
 * @param regAddr registers to read from
 * @param length  number of bytes to read
 * @param data    container for byte value read from device
 * @return        number of bytes read (0 == failure)
 */
int8_t I2Clib::readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
   int8_t status = 0;

   Wire.beginTransmission(devAddr);
   Wire.write(regAddr);
   Wire.endTransmission();

   Wire.beginTransmission(devAddr);
   Wire.requestFrom(devAddr, length);

   while(Wire.available() && status < length)
   {
       data[status] = Wire.read();
       status++;
   }

   Wire.endTransmission();

   return status;
}

/** Write a single bit to an 8-bit device register.
 * @param devAddr   I2c slave device address
 * @param regAddr   register to write to
 * @param bitPos    position of the bit to write to
 * @param data      bit value to write
 * @return          status of operation (true == success)
 */
bool I2Clib::writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, uint8_t data)
{
    uint8_t temp;
    readByte(devAddr, regAddr, &temp);
    temp = (data != 0) ? (temp | (1 << bitPos)) : (temp & ~(1 << bitPos));
    return writeByte(devAddr, regAddr, temp);
}

/** Write multiple bits in an 8-bit device register.
 * @param devAddr   I2C slave device address
 * @param regAddr   register to write to
 * @param bitStart  first bit position to write (0-7)
 * @param length    number of bits to write (not more than 8)
 * @param data      value to write
 * @return          status of operation (true == success)
 */
bool I2Clib::writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data)
// 76543210 register bit numbers
//      010 value to write
//    xxx   bitStart = 4; length = 3;
// 01000000 shift data left
// 00001000 shift data right, all other bits are 0
// 11100011 mask byte
// 01010101 byte read
// 01000011 byte masked
// 01001011 byte | data
{
    uint8_t temp;
    if (readByte(devAddr, regAddr, &temp) != 0)
    {
        uint8_t mask = (0xFF << (bitStart +1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        temp &= mask;
        temp |= data;
        return writeByte(devAddr, regAddr, temp);
    }
    else
    {
        return false;
    }
}

/** Write single byte to an 8-bit device register.
 * @param devAddr   I2C slave device address
 * @param regAddr   register to write to
 * @param data      new byte value to write
 * @return          status of operation (true = success)
 */
bool I2Clib::writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    return writeBytes(devAddr, regAddr, 1, &data);
}

/** Write single byte to an 8-bit device register.
 * @param devAddr   I2C slave device address
 * @param regAddr   register to write to
 * @param length    number of bytes to write
 * @param data      buffer to copy new data from
 * @return          status of operation (true = success)
 */
 bool I2Clib::writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data)
 {
    Wire.beginTransmission(devAddr);
    Wire.write(regAddr);

    for (uint8_t i = 0; i < length; i++)
    {
        Wire.write(data[i]);

    }

    uint8_t status = Wire.endTransmission();

    return !status;
 }
