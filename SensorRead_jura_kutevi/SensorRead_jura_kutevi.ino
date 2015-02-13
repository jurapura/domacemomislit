#include <Wire.h>
#include "config.h" 

unsigned long time,sTime,ms,interval,vrijeme1,vrijeme2,brojac;
byte prviput;
float dT;
float gx,gy,gz,gyr_pitch,gyr_roll,gyr_yaw;


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

