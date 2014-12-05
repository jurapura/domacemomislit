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
  writeTo(HMC5883L_W, CONFIG_A, 0x78); // [6:5] samples averaged (00=1, 01=2, 10=4, 11=8), [4:2] rate
  writeTo(HMC5883L_W, CONFIG_B, 0x20); // [7:5] gain (0.88, 1.3, 1.9, 2.5, 4.0, 4.7, 5.6, 8.1), rest must be 0
  writeTo(HMC5883L_W, MODE_REG, 0x01); // [1:0] operating mode single (01) or continuous mode (00)
  delay(100);
}

void testSensor()
{
  writeTo(HMC5883L_W, CONFIG_A, 0x70);
  writeTo(HMC5883L_W, CONFIG_B, 0xA0);
  writeTo(HMC5883L_W, MODE_REG, 0x00);
  delay(100);
}

void readSensor()
{
  byte readings[5];
  float data[2];
  readFrom(HMC5883L_W, DATA_OUTPUT_X_H, 6, readings);
  
  data[0] = readings[0]<<8 | readings[1];
  data[1] = readings[2]<<8 | readings[3];
  data[2] = readings[4]<<8 | readings[5];
  
  Serial.print("MagX = "); Serial.print(data[0]/1090.0);
  Serial.print(" | MagZ = "); Serial.print(data[1]/1090.0);
  Serial.print(" | MagY = "); Serial.println(data[2]/1090.0);
}

void readSensor1()
{
  byte readings[5];
  float data[2];
  
  Wire.beginTransmission(HMC5883L_R);
  Wire.write(0x06);
  Wire.endTransmission();
  
  delay(10);
  int i = 0;
  while(Wire.available() && i<6)
  {
    readings[i]=Wire.read();
    i++;
  }
  
  Wire.beginTransmission(HMC5883L_W);
  Wire.write(0x03);
  Wire.endTransmission();
  
  data[0] = readings[0]<<8 | readings[1];
  data[1] = readings[2]<<8 | readings[3];
  data[2] = readings[4]<<8 | readings[5];
  
  Serial.print("MagX = "); Serial.print(data[0]/1090.0);
  Serial.print(" | MagZ = "); Serial.print(data[1]/1090.0);
  Serial.print(" | MagY = "); Serial.println(data[2]/1090.0);
}
  
  
