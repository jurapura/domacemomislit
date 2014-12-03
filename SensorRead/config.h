//################################
//###     MPU-6050 defines     ###
//################################

#define MPU 0x68 // MPU-6050 I2C address
#define CONFIG 0x1A // [5:3] EXT_SYNC_SET, [2:0] DigitalLowPassFilter
#define GYRO_CONFIG 0x1B // [4:3] FS_SEL Select gyro range 0->250, 1->500, 2->1000, 3->2000 deg/s
#define ACCEL_CONFIG 0x1C // [4:3] AFS_SEL Select gyro range 0->2g, 1->4g, 2->8g, 3->16g
#define ACCEL_XOUT_H 0x3B // Accel x reading [15:8]
#define ACCEL_XOUT_L 0x3C // Accel x reading [7:0]
#define ACCEL_YOUT_H 0x3D // Accel y reading [15:8]
#define ACCEL_YOUT_L 0x3E // Accel y reading [7:0]
#define ACCEL_ZOUT_H 0x3F // Accel z reading [15:8]
#define ACCEL_ZOUT_L 0x40 // Accel z reading [7:0]
#define TEMP_OUT_H 0x41 // Temperature measurement [15:8]
#define TEMP_OUT_L 0x42 // Temperature measurement [7:0]
#define GYRO_XOUT_H 0x43 // Gyro x reading [15:8]
#define GYRO_XOUT_L 0x44 // Gyro x reading [7:0]
#define GYRO_YOUT_H 0x45 // Gyro y reading [15:8]
#define GYRO_YOUT_L 0x46 // Gyro y reading [7:0]
#define GYRO_ZOUT_H 0x47 // Gyro z reading [15:8]
#define GYRO_ZOUT_L 0x48 // Gyro z reading [7:0]
#define SIGNAL_PATH_RESET 0x68 // [2] Gyro reset, [1] Accel reset, [0] Temp reset
#define USR_CTRL 0x6A // [0] Reset all
#define PWR_MGMT 0x6B // [7] device_reset, [6] sleep {1 for sleep}

//################################
//###     HMC5883L defines     ###
//################################

#define CONFIG_A 0x00 // Configuration register A
#define CONFIG_B 0x01 // Configuration register B
#define MODE_REG 0x02 // Mode configuration register
#define DATA_OUTPUT_X_H 0x03 // MSB X Data register
#define DATA_OUTPUT_X_L 0x04 // LSB X Data register
#define DATA_OUTPUT_Z_H 0x05 // MSB Z Data register
#define DATA_OUTPUT_Z_L 0x06 // LSB Z Data register
#define DATA_OUTPUT_Y_H 0x07 // MSB Y Data register
#define DATA_OUTPUT_y_L 0x08 // LSB Y Data register
#define STATUS 0x09 // Status register
#define ID_A 0x10 // Id register A
#define ID_B 0x11 // Id register B
#define ID_C 0x12 // Id register C
