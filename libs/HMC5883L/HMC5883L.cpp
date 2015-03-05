/* Library for basic abstraction of features implemented in HMC5883L magnetometer via Arduino
I2C communication */

#include <HMC5883L.h>

/** Default constructor.
*/
HMC5883L::HMC5883L()
{
	x_scale = 1.0F;
	y_scale = 1.0F;
	z_scale = 1.0F;
}

/** Initialize HMC5883L magnetometer with 
	-75Hz data output rate, 
	-Continuous mode, 
	-± 1.3 Ga range, 
	-8 samples averaged, 
	-no bias.
 */
void HMC5883L::init()
{
	delay(5); // HMC5883L needs 5ms delay between power on and setup.

	// 75Hz, Continuous mode, ± 1.3 Ga, 8 samples averaged, no bias
	I2Clib::writeByte(HMC5883L_ADD, HMC5883L_CONF_REG_A, 0x78);
	I2Clib::writeByte(HMC5883L_ADD, HMC5883L_MODE_REG, 0x00);
}

/** Sets the Continuous Measurement Mode.
 *	@return	Returns the status of the operation (true == success)
 */
bool HMC5883L::setContMode()
{
	return I2Clib::writeByte(HMC5883L_ADD, HMC5883L_MODE_REG, 0x00);
}
 
/** Sets the Idle Mode.
 * 	@return Returns the status of the operation (true == success)
 */
bool HMC5883L::setIdleMode()
{
	return I2Clib::writeByte(HMC5883L_ADD, HMC5883L_MODE_REG, 0x02);
}

/** Sets the Single Measurement Mode.
 * 	@return Returns the status of the operation (true == success)
 */
bool HMC5883L::setSingleMode()
{
	return I2Clib::writeByte(HMC5883L_ADD, HMC5883L_MODE_REG, 0x01);
}

/** Reads the current operating mode of the device.
 * 	@return Returns the operating mode of the device (0 - Cont, 1 - Single, 2 - Idle)
 */
uint8_t HMC5883L::getOperatingMode()
{
	I2Clib::readBits(HMC5883L_ADD, HMC5883L_MODE_REG, 1, 2, temp);
	return temp;
}

/** Sets Data output rate of the device.
	0 -> 0.75Hz 
	1 ->  1.5Hz
	2 ->    3Hz
	3 ->  7.5Hz
	4 ->   15Hz (Default)
	5 ->   30Hz
	6 ->   75Hz
 * @param   data set the data output rate (0-6)
 * @return  returns the status of write operation (true == success)
 */
bool HMC5883L::setDataOutRate(uint8_t value)
{
    if data > 6
    {
        data = 6;
    }
    else if data < 0
    {
        data = 0;
    }
    return I2Clib::writeBits(HMC5883L_ADD, HMC5883L_CONF_REG_A, 4, 3, value);
}

/** Gets Data output range of the device.
    0 -> 0.75Hz 
    1 ->  1.5Hz
    2 ->    3Hz
    3 ->  7.5Hz
    4 ->   15Hz
    5 ->   30Hz
    6 ->   75Hz
 * @return  returns the data output range of the device (0-6)
 */
uint8_t HMC5883L::getDataOutRate()
{
    I2Clib::readBits(HMC5883L_ADD, HMC5883L_CONF_REG_A, 4, 3, temp);
    return temp;
}

/** Sets the register value that determines the number of samples averaged.
    0 -> 1
    1 -> 2
    2 -> 4
    3 -> 8
 * @param   data set the number of samples averaged
 * @return  returns the status of write operation (true == success)
 */
bool HMC5883L::setSamplesAvg(uint8_t value)
{
    if data > 3
    {
        data = 3;
    }
    else if data < 0
    {
        data = 0;
    }
    return I2Clib::writeBits(HMC5883L_ADD, HMC5883L_CONF_REG_A, 6, 2, value);
}

/** Gets the register value that determines the number of samples averaged
    0 -> 1
    1 -> 2
    2 -> 4
    3 -> 8
 * @return  returns the register value that determines the number of samples averaged
 */
uint8_t HMC5883L::getSamplesAvg()
{
    I2Clib::readBits(HMC5883L_ADD, HMC5883L_CONF_REG_A, 6, 2, temp)
    return temp;
}

/** Sets the normal bias measurement flow of the device
 * @return returns the status of the write operation (true == success)
 */
bool HMC5883L::setNormalBias()
{
	return I2Clib::writeBits(HMC5883L_ADD, HMC5883L_CONF_REG_A, 1, 2, 0);
}

/** Sets the negative bias measurement flow of the device
 * @return 	returns the status of the write operation (true == success)
 */
bool HMC5883L::setPositiveBias()
{
	return I2Clib::writeBits(HMC5883L_ADD, HMC5883L_CONF_REG_A, 1, 2, 1);
}

/** Sets the positive bias measurement flow of the device
 * @return 	returns the status of the write operation (true == success)
 */
bool HMC5883L::setNegativeBias()
{
	return I2Clib::writeBits(HMC5883L_ADD, HMC5883L_CONF_REG_A, 1, 2, 2);
}

/** Gets the current bias measurement flow of the device
    <b00> Continuous-Measurement Mode.
    <b01> Single-Measurement Mode. When single-measurement
            mode is selected, device performs a single measurement, sets RDY
            high and returned to idle mode. Mode register returns to idle mode
            bit values. The measurement remains in the data output register and
            RDY remains high until the data output register is read or another
            measurement is performed. 
    <b10> Idle mode.
    <b11> Idle mode. 
 * @return  returns the current bias measurement flow of the device (0-3)
 */
uint8_t HMC5883L::getBias()
{
    I2Clib::readBits(HMC5883L_ADD, HMC5883L_CONF_REG_A, 1, 2, temp)
    return temp;
}

/** Sets the gain in configuration register B
    Value  Range     Gain
    <b000> ± 0.88 Ga 1370
    <b001> ± 1.3  Ga 1090 (default)
    <b010> ± 1.9  Ga 820 
    <b011> ± 2.5  Ga 660 
    <b100> ± 4.0  Ga 440
    <b101> ± 4.7  Ga 390
    <b110> ± 5.6  Ga 330
    <b111> ± 8.1  Ga 230
 * @param   data set the gain of the device
 * @return  returns the status of write operation (true == success)
 */
bool HMC5883L::setGain(uint8_t value)
{
    return I2Clib::writeBits(HMC5883L_ADD, HMC5883L_CONF_REG_B, 7, 3, value);
}

/** Gets the gain in configuration register B
    Value  Range     Gain
    <b000> ± 0.88 Ga 1370
    <b001> ± 1.3  Ga 1090 (default)
    <b010> ± 1.9  Ga 820 
    <b011> ± 2.5  Ga 660 
    <b100> ± 4.0  Ga 440
    <b101> ± 4.7  Ga 390
    <b110> ± 5.6  Ga 330
    <b111> ± 8.1  Ga 230
 * @return  returns the status of write operation (true == success)
 */
uint8_t HMC5883L::getGain()
{
    I2Clib::readBits(HMC5883L_ADD, HMC5883L_CONF_REG_B, 7, 3, temp);
    return temp;
}

