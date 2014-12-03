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
}

void initSensor()
{
  writeTo(MPU, PWR_MGMT, 0x00); // Resets the sensor and puts into wake up mode
  writeTo(MPU, CONFIG, 0x00); // 0-6 => sets the dlpf, 0->disable, ..., 6->5Hz
  writeTo(MPU, GYRO_CONFIG, 0x00); // 0x00, 0x08, 0x10, 0x18 => 250, 500, 1000, 2000 deg/s
  writeTo(MPU, ACCEL_CONFIG, 0x00); // 0x00, 0x08, 0x10, 0x18 => 2, 4, 8, 16g
}

void readSensor()
{
  byte readings[13];
  float data[6];
  readFrom(MPU, ACCEL_XOUT_H, 14, readings);
  
  data[0] = readings[0]<<8 | readings[1];
  data[1] = readings[2]<<8 | readings[3];
  data[2] = readings[4]<<8 | readings[5];
  data[3] = readings[6]<<8 | readings[7];
  data[4] = readings[8]<<8 | readings[9];
  data[5] = readings[10]<<8 | readings[11];
  data[6] = readings[12]<<8 | readings[13];
  
  Serial.print("AcX = "); Serial.print(data[0]/16384.0);
  Serial.print(" | AcY = "); Serial.print(data[1]/16384.0);
  Serial.print(" | AcZ = "); Serial.print(data[2]/16384.0);
  Serial.print(" | Tmp = "); Serial.print(data[3]/340.00+36.53); 
  Serial.print(" | GyX = "); Serial.print(data[4]/131.0);
  Serial.print(" | GyY = "); Serial.print(data[5]/131.0);
  Serial.print(" | GyZ = "); Serial.println(data[6]/131.0);
}
