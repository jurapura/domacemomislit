#include <Wire.h>
#include "config.h" 

unsigned long time,sTime,ms,interval,vrijeme1,vrijeme2,brojac;


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
  interval=20000;
  brojac=0;

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

