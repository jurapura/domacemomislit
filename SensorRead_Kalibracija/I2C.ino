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
  sTime=time;*/
  
  float norma=sqrt(pow(data_gyro_accel[1],2)+pow(data_gyro_accel[2],2)+pow(data_gyro_accel[0],2));
  float ax=data_gyro_accel[0]/norma;
  float ay=data_gyro_accel[1]/norma;
  float az=data_gyro_accel[2]/norma;
  float acc_roll=atan2(-ax,az)*(180/3.14);
  float acc_pitch=atan2(ay,sqrt(pow(ax,2)+pow(az,2)))*(180/3.14);
  

  
  if (prviput) {
    comp_pitch=acc_pitch;
    comp_roll=acc_roll;
    gyr_yaw=0;
    prviput=0;
  }
    
  gx=data_gyro_accel[4]/131.0-(gyro_bias[0]);
  gy=data_gyro_accel[5]/131.0-(gyro_bias[1]);
  gz=data_gyro_accel[6]/131.0-gyro_bias[2];
  
  dgyr_roll=gy;
  dgyr_pitch=gx*cos(comp_roll*(3.1415/180))+gz*sin(comp_roll*(3.1415/180));
  
   comp_roll=k*(comp_roll+dgyr_roll*dT)+(1-k)*(acc_roll);
 comp_pitch=k*(comp_pitch+dgyr_pitch*dT)+(1-k)*(acc_pitch);
 
  gyr_yaw=gyr_yaw+(gy*sin(comp_pitch*(3.1415/180))+gz*cos(comp_roll*(3.1415/180))*cos(comp_pitch*(3.1415/180))-gx*cos(comp_pitch*(3.1415/180))*sin(comp_roll*(3.1415/180)))*dT;
  
  
  if (gyr_yaw>180){gyr_yaw=gyr_yaw-360;}
  if (gyr_yaw<-180){gyr_yaw=gyr_yaw+360;}
  
 

    Serial.print(gx,3);
  Serial.print(",");
  Serial.print(gy,3);
  Serial.print(",");
  Serial.println(gz,3);

// Serial.print(data_gyro_accel[0]/16384.0,1);
// Serial.print(","); Serial.print(data_gyro_accel[1]/16384.0,1);
// Serial.print(","); Serial.println(data_gyro_accel[2]/16384.0,1);Serial.print(",");

//  Serial.print(comp_roll,0);
//   Serial.print(",");
//   Serial.print(comp_pitch,0);
//   Serial.print(",");
//   Serial.print(gyr_yaw,0); //x
//   Serial.println();    
  //
  
  /* Serial.print(","); Serial.print(acc_roll,4);
    Serial.print(","); Serial.print(acc_pitch,4);
  Serial.print(","); Serial.print(brojac%10);//y
  Serial.print(","); Serial.println(ms%100);//y*/
  
  

}

void gyro_inicijalizacija(){
  delay(5000);
  int gyro_bias_avg_num=30000;
  int gyro_full_circle_number=5;
  
  byte readings_gyro_accel[13];
  float data_gyro_accel[6];
  float gyro_bias_last[3]={0,0,0};
  float max_bias_steady_factor=2.0;
  
  
  while (counter<gyro_bias_avg_num){
    readFrom(MPU, ACCEL_XOUT_H, 14, readings_gyro_accel);
  data_gyro_accel[0] = readings_gyro_accel[0]<<8 | readings_gyro_accel[1];
  data_gyro_accel[1] = readings_gyro_accel[2]<<8 | readings_gyro_accel[3];
  data_gyro_accel[2] = readings_gyro_accel[4]<<8 | readings_gyro_accel[5];
  data_gyro_accel[3] = readings_gyro_accel[6]<<8 | readings_gyro_accel[7];
  data_gyro_accel[4] = readings_gyro_accel[8]<<8 | readings_gyro_accel[9];
  data_gyro_accel[5] = readings_gyro_accel[10]<<8 | readings_gyro_accel[11];
  data_gyro_accel[6] = readings_gyro_accel[12]<<8 | readings_gyro_accel[13];
    
    if (counter>0) {

       if (abs(data_gyro_accel[4])<(max_bias_steady_factor*abs(gyro_bias_last[0])) || abs(data_gyro_accel[5])<(max_bias_steady_factor*abs(gyro_bias_last[1])) || abs(data_gyro_accel[6])<(max_bias_steady_factor*abs(gyro_bias_last[2]))){
           gyro_bias[0]=gyro_bias[0]+data_gyro_accel[4];
           gyro_bias[1]=gyro_bias[1]+data_gyro_accel[5];
           gyro_bias[2]=gyro_bias[2]+data_gyro_accel[6];
           gyro_bias_last[0]=gyro_bias[0]/(counter+1);
           gyro_bias_last[1]=gyro_bias[1]/(counter+1);
           gyro_bias_last[2]=gyro_bias[2]/(counter+1);
           counter=counter+1;
           Serial.print(counter);Serial.print("   ");Serial.println("1");
         }
       else{
           counter=0;
           for(int i = 0; i < 3 ; i++){
             gyro_bias_last[i]=0.0;
             gyro_bias[i]=0.0;}
           Serial.print(counter);Serial.print("   ");Serial.println("2");
           delay(200);
  
           
         }
        }
        else{
           gyro_bias[0]=data_gyro_accel[4];
           gyro_bias[1]=data_gyro_accel[5];
           gyro_bias[2]=data_gyro_accel[6];
           gyro_bias_last[0]=gyro_bias[0];
           gyro_bias_last[1]=gyro_bias[1];
           gyro_bias_last[2]=gyro_bias[2];
           counter=counter+1;
           Serial.print(counter);Serial.print("   ");Serial.println("3");
        }
    
    
  
  }
  
  
  for(int i = 0; i < 3 ; i++){
             gyro_bias[i]=gyro_bias[i]/(gyro_bias_avg_num*131.0);}
  Serial.print(gyro_bias[0],3);
  Serial.print("   ");
  Serial.print(gyro_bias[1],3);
  Serial.print("   ");
  Serial.println(gyro_bias[2],3);
  delay(2000);
  Serial.println("Kalibracija x gaina, kad si spreman zarotirati za 5 krugova stisni s, kad si gotov stisni e");
  char decision='0';
  while (!(decision=='s')) {
    while(!Serial.available()){
      }
      decision=Serial.read();
      Serial.println(decision);
   }
   
   unsigned long time2;
   unsigned long time1=micros();
   double deltaT=0.0;
   double integral=0.0;
   while(decision!='e'){
           readFrom(MPU, ACCEL_XOUT_H, 14, readings_gyro_accel);
           data_gyro_accel[0] = readings_gyro_accel[0]<<8 | readings_gyro_accel[1];
           data_gyro_accel[1] = readings_gyro_accel[2]<<8 | readings_gyro_accel[3];
           data_gyro_accel[2] = readings_gyro_accel[4]<<8 | readings_gyro_accel[5];
           data_gyro_accel[3] = readings_gyro_accel[6]<<8 | readings_gyro_accel[7];
           data_gyro_accel[4] = readings_gyro_accel[8]<<8 | readings_gyro_accel[9];
           data_gyro_accel[5] = readings_gyro_accel[10]<<8 | readings_gyro_accel[11];
           data_gyro_accel[6] = readings_gyro_accel[12]<<8 | readings_gyro_accel[13];
           
           
           time2=micros();
           deltaT=(time2-time1)/1000000.0;
           time1=time2;
           
           integral=integral+deltaT*(data_gyro_accel[4]/131.0-gyro_bias[0]);
           Serial.print(integral,5);
               Serial.print("         ");
    Serial.println(deltaT,6);
           if (Serial.available()>0){decision=Serial.read();}
    }
    gyro_gain[0]=integral/(360.0*gyro_full_circle_number);
    Serial.print(integral);Serial.print("     ");Serial.println(gyro_gain[0],6);
    
    Serial.println("Kalibracija y gaina, kad si spreman zarotirati za 5 krugova stisni s, kad si gotov stisni e");
  decision='0';
  while (!(decision=='s')) {
    while(!Serial.available()){
      }
      decision=Serial.read();
      Serial.println(decision);
   }
   
   
   time1=micros();
   deltaT=0.0;
   integral=0.0;
   while(decision!='e'){
           readFrom(MPU, ACCEL_XOUT_H, 14, readings_gyro_accel);
           data_gyro_accel[0] = readings_gyro_accel[0]<<8 | readings_gyro_accel[1];
           data_gyro_accel[1] = readings_gyro_accel[2]<<8 | readings_gyro_accel[3];
           data_gyro_accel[2] = readings_gyro_accel[4]<<8 | readings_gyro_accel[5];
           data_gyro_accel[3] = readings_gyro_accel[6]<<8 | readings_gyro_accel[7];
           data_gyro_accel[4] = readings_gyro_accel[8]<<8 | readings_gyro_accel[9];
           data_gyro_accel[5] = readings_gyro_accel[10]<<8 | readings_gyro_accel[11];
           data_gyro_accel[6] = readings_gyro_accel[12]<<8 | readings_gyro_accel[13];
           
           
           time2=micros();
           deltaT=(time2-time1)/1000000.0;
           time1=time2;
           
           integral=integral+deltaT*(data_gyro_accel[5]/131.0-gyro_bias[1]);
           Serial.print(integral,5);
               Serial.print("         ");
    Serial.println(deltaT,6);
           if (Serial.available()>0){decision=Serial.read();}
    }
     gyro_gain[1]=integral/(360.0*gyro_full_circle_number);
    Serial.print(integral);Serial.print("     ");Serial.println(gyro_gain[1],6);
    
        Serial.println("Kalibracija z gaina, kad si spreman zarotirati za 5 krugova stisni s, kad si gotov stisni e");
  decision='0';
  while (!(decision=='s')) {
    while(!Serial.available()){
      }
      decision=Serial.read();
      Serial.println(decision);
   }
   
  
   time1=micros();
   deltaT=0.0;
   integral=0.0;
   while(decision!='e'){
           readFrom(MPU, ACCEL_XOUT_H, 14, readings_gyro_accel);
           data_gyro_accel[0] = readings_gyro_accel[0]<<8 | readings_gyro_accel[1];
           data_gyro_accel[1] = readings_gyro_accel[2]<<8 | readings_gyro_accel[3];
           data_gyro_accel[2] = readings_gyro_accel[4]<<8 | readings_gyro_accel[5];
           data_gyro_accel[3] = readings_gyro_accel[6]<<8 | readings_gyro_accel[7];
           data_gyro_accel[4] = readings_gyro_accel[8]<<8 | readings_gyro_accel[9];
           data_gyro_accel[5] = readings_gyro_accel[10]<<8 | readings_gyro_accel[11];
           data_gyro_accel[6] = readings_gyro_accel[12]<<8 | readings_gyro_accel[13];
           
           
           time2=micros();
           deltaT=(time2-time1)/1000000.0;
           time1=time2;
           
           integral=integral+deltaT*(data_gyro_accel[6]/131.0-gyro_bias[2]);
           Serial.print(integral,5);
               Serial.print("         ");
    Serial.println(deltaT,6);
           if (Serial.available()>0){decision=Serial.read();}
    }
     gyro_gain[2]=integral/(360.0*gyro_full_circle_number);
    Serial.print(integral);Serial.print("     ");Serial.println(gyro_gain[2],6);
    

   
  
  
  delay(20000);
  

  
  
}
