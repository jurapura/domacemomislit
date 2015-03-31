#include <Wire.h>
#include "config.h" 
#include <math.h> 

unsigned long time,sTime,ms,interval,vrijeme1,vrijeme2,brojac;
byte prviput;
float dT;
float gx,gy,gz,dgyr_pitch,dgyr_roll,gyr_yaw,comp_pitch,comp_roll,k;
double gyro_bias[3]={0,0,0};
double gyro_gain[3]={0,0,0};
int counter=0;


//###############################
//###       Main program      ###
//###############################

void setup()
{
  Serial.begin(57600);
  Wire.begin();
  initSensors(); 
  sTime=millis();
  vrijeme1=0;
  dT=0.02;
  interval=20000;
  brojac=0;
  prviput=1;
  k=0.95;
  
  gyro_inicijalizacija();

}

void loop()
{ 
  vrijeme2=micros();
  
  if(vrijeme2-vrijeme1>interval){
    vrijeme1=vrijeme2;
    readSensor();
  }
//  readSensor();
//  delay(20);

}

