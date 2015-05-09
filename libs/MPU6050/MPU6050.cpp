/* Library for basic abstraction of some features implemented in MPU6050 sensor via
I2C communication */

#include "MPU6050.h"

/** Default constructor.
 */
MPU6050::MPU6050()
{

}


void MPU6050::init()
{
	delay(10);
    // Set I2C bypass to enable.
    // This connects the auxiliary I2C bus with the main one.
    // When this bit is equal to 1 and I2C_MST_EN (Register 106 bit[5]) is equal to
    // 0, the host application processor will be able to directly access the auxiliary
    // I2C bus of the MPU-60X0.
    I2Clib::writeBit(MPU6050_ADD, MPU6050_INT_PIN_CFG, 1, 1);
    I2Clib::writeBit(MPU6050_ADD, MPU6050_USER_CTRL, 5, 0);

    // Sensor is in sleep by default.
    // Must set bit 6 in PWR_MGMT_1 register to 0.
    I2Clib::writeBit(MPU6050_ADD, MPU6050_PWR_MGMT_1, 6, 0);

}

/**
 * @brief Sets the SMPLRT_DIV register uint8_t value
 * @details register value is used to calculate Sample rate
 * Sample rate = Gyro Out Rate / (1 + SMPLRT_DIV)
 *
 * @param value uint8_t value to be written to SMPLRT_DIV register
 * @return returns the status of the operation, 1 == success
 */
bool MPU6050::setSampleRateDiv(uint8_t value)
{
    return I2Clib::writeByte(MPU6050_ADD, MPU6050_SMPLRT_DIV, value);
}

/**
 * @brief Gets the uint8_t value stored in SMPLRT_DIV register
 * @details register value is used to calculate Sample rate
 * Sample rate = Gyro Out Rate / (1 + SMPLRT_DIV)
 *
 * @return uint8_t value from SMPLRT_DIV register, returns 0 for reading error
 */
uint8_t MPU6050::getSampleRateDiv()
{
    if (I2Clib::readByte(MPU6050_ADD, MPU6050_SMPLRT_DIV, buffer))
    {
        return buffer[0];
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Sets the value of the Digital Low Pass Filter setting
 * @details This register configures the Digital Low Pass Filter (DLPF) setting for
 * both the gyroscopes and accelerometers.
 *
 * The DLPF is configured by bits [2:0]. Sensors are filtered according to the table:
 * <value>     | <acc_setting>      | <gyro_setting>
 * ------------+--------------------+--------------
 *     0       | 260Hz              | 256Hz
 *     1       | 184Hz              | 188Hz
 *     2       | 94Hz               | 98Hz
 *     3       | 44Hz               | 42Hz
 *     4       | 21Hz               | 20Hz
 *     5       | 10Hz               | 10Hz
 *     6       | 5Hz                | 5Hz
 *     7       | -                  | -
 *
 * @param value uint8_t value to be written to the register
 * @return returns the status of the opearation (true == success)
 */
bool MPU6050::setDLPF(uint8_t value)
{
    if (value > 6)
    {
        value = 6;
    }
    return I2Clib::writeBits(MPU6050_ADD, MPU6050_CONFIG, 2, 3, value);
}

/**
 * @brief Gets the value of the Digital Low Pass Filter setting
 * @details This register configures the Digital Low Pass Filter (DLPF) setting for
 * both the gyroscopes and accelerometers.
 *
 * The DLPF is configured by bits [2:0]. Sensors are filtered according to the table:
 * <value>     | <acc_setting>      | <gyro_setting>
 * ------------+--------------------+--------------
 *     0       | 260Hz              | 256Hz
 *     1       | 184Hz              | 188Hz
 *     2       | 94Hz               | 98Hz
 *     3       | 44Hz               | 42Hz
 *     4       | 21Hz               | 20Hz
 *     5       | 10Hz               | 10Hz
 *     6       | 5Hz                | 5Hz
 *     7       | -                  | -
 *
 * @return returns the setting of the Digital Low Pass Filter or 7 if read error
 */
uint8_t MPU6050::getDLPF()
{
    if (I2Clib::readBits(MPU6050_ADD, MPU6050_CONFIG, 2, 3, buffer))
    {
        return buffer[0];
    }
    else
    {
        return 7;
    }
}

/**
 * @brief Sets the value of the gyroscopes' full scale range register
 * @details This register is used to configure the gyroscopes’ full scale range
 * according to the following table:
 *
 * <value>  | <full scale range>
 * ---------+--------------------
 *     0    | ± 250 °/s
 *     1    | ± 500 °/s
 *     2    | ± 1000 °/s
 *     3    | ± 2000 °/s
 *
 * It is configured by bits [4:3]
 *
 * @param value uint8_t [0:3] Selects the full scale range of gyroscopes
 * @return returns the status of the operation (true == success)
 */
bool setGyroConfig(uint8_t value)
{
    if (value > 3)
    {
        value = 3;
    }
    return I2Clib::writeBits(MPU6050_ADD, MPU6050_GYRO_CONFIG, 4, 2, value);
}

/**
 * @brief Gets the value of the gyroscopes' full scale range register
 * @details This register is used to configure the gyroscopes’ full scale range
 * according to the following table:
 *
 * <value>  | <full scale range>
 * ---------+--------------------
 *     0    | ± 250 °/s
 *     1    | ± 500 °/s
 *     2    | ± 1000 °/s
 *     3    | ± 2000 °/s
 *
 * It is configured by bits [4:3]
 *
 * @return returns the value of the gyroscopes' full scale range setting [0:3]
 * or 4 for reading error
 */
uint8_t MPU6050::getGyroConfig()
{
    if (I2Clib::readBits(MPU6050_ADD, MPU6050_GYRO_CONFIG, 4, 2, buffer))
    {
        return buffer[0];
    }
    else
    {
        return 4;
    }
}

/**
 * @brief Sets the value of the accelerometers' full scale range register
 * @details This register is used to configure the accelerometers’ full scale range
 * according to the following table:
 *
 * <value>  | <full scale range>
 * ---------+--------------------
 *     0    | ± 2g
 *     1    | ± 4g
 *     2    | ± 8g
 *     3    | ± 16g
 *
 * It is configured by bits [4:3]
 *
 * @param value uint8_t [0:3] Selects the full scale range of accelerometers
 * @return returns the status of the operation (true == success)
 */
bool MPU6050::setAccelConfig(uint8_t value)
{
    if (value > 3)
    {
        value = 3;
    }
    return I2Clib::writeBits(MPU6050_ADD, MPU6050_ACCEL_CONFIG, 4, 2, value);
}

/**
 * @brief Gets the value of the accelerometers' full scale range register
 * @details This register is used to configure the accelerometers’ full scale range
 * according to the following table:
 *
 * <value>  | <full scale range>
 * ---------+--------------------
 *     0    | ± 2g
 *     1    | ± 4g
 *     2    | ± 8g
 *     3    | ± 16g
 *
 * It is configured by bits [4:3]
 *
 * @param value uint8_t returns the value of the accelerometers full scale
 * range setting [0:3] or 4 for reading error
 * @return returns the status of the operation (true == success)
 */
uint8_t MPU6050::getAccelConfig()
{
    if (I2Clib::readBits(MPU6050_ADD, MPU6050_ACCEL_CONFIG, 4, 2, buffer))
    {
        return buffer[0];
    }
    else
    {
        return 4;
    }
}

/**
 * @brief These registers store the most recent accelerometer measurements.
 *
 * @details
 * Accelerometer measurements are written to these registers at the Sample Rate
 * as defined in Register 25, SMPLRT_DIV.
 *
 * Each 16-bit accelerometer measurement has a full scale defined in ACCEL_FS
 * (Register 28, ACCEL_CONFIG). For each full scale setting,
 * the accelerometers' sensitivity per LSB in ACCEL_xOUT is shown in the table below:
 *
 * <value> | Full Scale Range | LSB Sensitivity
 * --------+------------------+----------------
 * 0       | +/- 2g           | 8192 LSB/mg
 * 1       | +/- 4g           | 4096 LSB/mg
 * 2       | +/- 8g           | 2048 LSB/mg
 * 3       | +/- 16g          | 1024 LSB/mg
 *
 * @param x 16-bit signed integer container for X-axis acceleration
 * @param y 16-bit signed integer container for Y-axis acceleration
 * @param z 16-bit signed integer container for Z-axis acceleration
 */
void MPU6050::getAcceleration(int16_t* x, int16_t* y, int16_t* z)
{
    I2Clib::readBytes(MPU6050_ADD, MPU6050_ACCEL_XOUT_H, 6, buffer);
    *x = (((int16_t) buffer[0]) << 8) | buffer[1];
    *y = (((int16_t) buffer[2]) << 8) | buffer[3];
    *z = (((int16_t) buffer[4]) << 8) | buffer[5];
}

/**
 * @brief These registers store the most recent temperature sensor measurement.
 * @details
 * Temperature measurements are written to these registers at the Sample Rate
 * as defined in Register 25, SMPLRT_DIV.
 *
 * The temperature in degrees C for a given register value may be computed as:
 *
 * Temp in degrees = (TEMP_OUT Register Value as a signed quantity)/340 + 36.53
 *
 * @param temp 16-bit signed integer container for temperature measurement
 */
int16_t MPU6050::getTemperature()
{
    I2Clib::readBytes(MPU6050_ADD, MPU6050_TEMP_OUT_H, 2, buffer);
    return (((int16_t) buffer[0]) << 8) | buffer[1];
}

/**
 * @brief These registers store the most recent gyroscope measurements.
 * @details Gyroscope measurements are written to these registers at the
 * Sample Rate as defined in Register 25, SMPLRT_DIV.
 *
 * Each 16-bit gyroscope measurement has a full scale defined in FS_SEL (Register 27).
 * For each full scale setting, the gyroscopes’ sensitivity per LSB in GYRO_xOUT is
 * shown in the table below:
 *
 * <value>| Full Scale Range   | LSB Sensitivity
 * -------+--------------------+----------------
 * 0      | +/- 250 degrees/s  | 131 LSB/deg/s
 * 1      | +/- 500 degrees/s  | 65.5 LSB/deg/s
 * 2      | +/- 1000 degrees/s | 32.8 LSB/deg/s
 * 3      | +/- 2000 degrees/s | 16.4 LSB/deg/s
 *
 * @param x 16-bit signed integer container for X-axis rotation
 * @param y 16-bit signed integer container for Y-axis rotation
 * @param z 16-bit signed integer container for Z-axis rotation
 */
void MPU6050::getRotationRate(int16_t* x, int16_t* y, int16_t* z)
{
    I2Clib::readBytes(MPU6050_ADD, MPU6050_GYRO_XOUT_H, 6, buffer);
    *x = (((int16_t) buffer[0]) << 8) | buffer[1];
    *y = (((int16_t) buffer[2]) << 8) | buffer[3];
    *z = (((int16_t) buffer[4]) << 8) | buffer[5];
}

/**
 * @brief Get raw 6-axis motion sensor readings (accel/gyro)
 * @details Get 6 16-bit singed integers. First three readings are X, Y and Z
 * accelerometer values and last three are X, Y and Z gyroscope values.
 *
 * @param ax 16-bit signed integer container for accelerometer X-axis value
 * @param ay 16-bit signed integer container for accelerometer Y-axis value
 * @param az 16-bit signed integer container for accelerometer Z-axis value
 * @param gx 16-bit signed integer container for gyroscope X-axis value
 * @param gy 16-bit signed integer container for gyroscope Y-axis value
 * @param gz 16-bit signed integer container for gyroscope Z-axis value
 */
void MPU6050::getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz)
{
    I2Clib::readBytes(MPU6050_ADD, MPU6050_ACCEL_XOUT_H, 14, buffer);
    *ax = (((int16_t)buffer[0]) << 8) | buffer[1];
    *ay = (((int16_t)buffer[2]) << 8) | buffer[3];
    *az = (((int16_t)buffer[4]) << 8) | buffer[5];
    *gx = (((int16_t)buffer[8]) << 8) | buffer[9];
    *gy = (((int16_t)buffer[10]) << 8) | buffer[11];
    *gz = (((int16_t)buffer[12]) << 8) | buffer[13];
}

/**
 * @brief Resets the sensors and their data registers.
 * @details Resets the signal paths for all sensors (gyro, accel and temp).
 * Also clears the sensor registers. This bit automatically clears to 0
 * after the reset has been triggered.
 */
void MPU6050::resetSensors()
{
    I2Clib::writeBit(MPU6050_ADD, MPU6050_USER_CTRL, 0, 1);
}

/**
 * @brief Resets all internal registers to their default values.
 * @details When set to 1, this bit resets all internal registers to their
 * default values. The bit automatically clears to 0 once the reset is done.
 */
void MPU6050::resetDevice()
{
    I2Clib::writeBit(MPU6050_ADD, MPU6050_PWR_MGMT_1, 7, 1);
}


bool MPU6050::testConnection()
{
    I2Clib::readByte(MPU6050_ADD, MPU6050_WHO_AM_I, buffer);
    return buffer[0] == 0x68;
}
