void writeTo(int DEVICE, byte address, byte val)
{
  Wire.beginTransmission(DEVICE);
  Wire.write(address);
  Wire.write(val);
  Wire.endTransmission();
}

void readFrom(int DEVICE, byte address, int num, byte data[])
{
  Wire.beginTransmission(DEVICE);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(DEVICE, num);
  
  int i = 0;
  while(Wire.available() && i<num)
  {
    data[i]=Wire.read();
    i++;
  }
  Wire.endTransmission();
}

void initSensors()
{
//  byte temp[0];
//  readFrom(MPU, BYPASS, 1, temp); // Reads from the address 55
//  Serial.print(temp[0]);
//  temp[0] |= 0x01;                   // enables i2c bypass
//  Serial.print(temp[0]);
  writeTo(MPU, PWR_MGMT, 0x00); // Resets the sensor and puts into wake up mode
  writeTo(MPU, BYPASS, 0x02);     // writes to address 55
  writeTo(MPU, USR_CTRL, 0x00);   // disables aux i2c
  delay(10);
  writeTo(MPU, CONFIG, 0x00); // 0-6 => sets the dlpf, 0->disable, ..., 6->5Hz
  writeTo(MPU, GYRO_CONFIG, 0x00); // 0x00, 0x08, 0x10, 0x18 => 250, 500, 1000, 2000 deg/s
  writeTo(MPU, ACCEL_CONFIG, 0x00); // 0x00, 0x08, 0x10, 0x18 => 2, 4, 8, 16g
  writeTo(HMC5883L, CONFIG_A, 0x70); // [6:5] samples averaged (00=1, 01=2, 10=4, 11=8), [4:2] rate
  writeTo(HMC5883L, CONFIG_B, 0xA0); // [7:5] gain (0.88, 1.3, 1.9, 2.5, 4.0, 4.7, 5.6, 8.1), rest => 0
  writeTo(HMC5883L, MODE_REG, 0x00); // [1:0] operating mode single (01) or continuous mode (00)
}

void readSensor()
{
  byte readings_gyro_accel[13];
  float data_gyro_accel[6];
  readFrom(MPU, ACCEL_XOUT_H, 14, readings_gyro_accel);
  
  data_gyro_accel[0] = readings_gyro_accel[0]<<8 | readings_gyro_accel[1];
  data_gyro_accel[1] = readings_gyro_accel[2]<<8 | readings_gyro_accel[3];
  data_gyro_accel[2] = readings_gyro_accel[4]<<8 | readings_gyro_accel[5];
  data_gyro_accel[3] = readings_gyro_accel[6]<<8 | readings_gyro_accel[7];
  data_gyro_accel[4] = readings_gyro_accel[8]<<8 | readings_gyro_accel[9];
  data_gyro_accel[5] = readings_gyro_accel[10]<<8 | readings_gyro_accel[11];
  data_gyro_accel[6] = readings_gyro_accel[12]<<8 | readings_gyro_accel[13];
  
  byte readings_mag[5];
  float data_mag[2];
  readFrom(HMC5883L, DATA_OUTPUT_X_H, 6, readings_mag);
  
  data_mag[0] = readings_mag[0]<<8 | readings_mag[1];
  data_mag[1] = readings_mag[2]<<8 | readings_mag[3];
  data_mag[2] = readings_mag[4]<<8 | readings_mag[5];  
  //time = millis();
 /* brojac=brojac+1;
  
  ms=time-sTime;
  sTime=time;
  
 /* float norma=pow(data_gyro_accel[1],2)+pow(data_gyro_accel[2],2)+pow(data_gyro_accel[3],2);
  float gx=data_gyro_accel[1]/norma;
  float gy=data_gyro_accel[2]/norma;
  float gz=data_gyro_accel[3]/norma;
  float acc_roll=atan2(gy,gz)*(180/3.14);
  float acc_pitch=atan2(-gx,sqrt(pow(gy,2)+pow(gz,2)))*(180/3.14);*/

// Serial.print(data_gyro_accel[0]/16384.0,1);
// Serial.print(","); Serial.print(data_gyro_accel[1]/16384.0,1);
// Serial.print(","); Serial.println(data_gyro_accel[2]/16384.0,1);Serial.print(",");
  Serial.print(data_gyro_accel[4]/131.0,4);
 Serial.print(","); Serial.print(data_gyro_accel[5]/131.0,4);
  Serial.print(","); Serial.println(data_gyro_accel[6]/131.0,4);
//  Serial.print(",");Serial.print(data_mag[0]/1090.0,4); //x
//  Serial.print(","); Serial.print(data_mag[1]/1090.0,4);//z
//  Serial.print(","); Serial.println(data_mag[2]/1090.0,4);
  
  /* Serial.print(","); Serial.print(acc_roll,4);
    Serial.print(","); Serial.print(acc_pitch,4);
  Serial.print(","); Serial.print(brojac%10);//y
  Serial.print(","); Serial.println(ms%100);//y*/
  
  

}
