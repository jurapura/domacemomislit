#include <Wire.h>

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

//###############################
//###       Main program      ###
//###############################

void setup()
{
  Wire.begin();
  initSensor(); //novo
  //writeTo(0x68, 0x6B, 0);
  Serial.begin(9600);
}

void loop()
{
  readSensor();
  delay(20);
}
