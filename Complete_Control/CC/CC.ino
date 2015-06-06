#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>

#include "quadControl.h"


quadControl quad;



unsigned long cycleTime,timeStart,timeEnd;


void setup()
{
    Wire.begin();
    Serial.begin(57600);
    Serial1.begin(57600);
    delay(5000);
    quad.init();
    quad.ESCcalibration();
    timeStart=0;
    cycleTime=20000; //time of cylce in microseconds

}

void loop()
{

timeEnd=micros();
if(timeEnd-timeStart>cycleTime){
    timeStart=timeEnd;
    
    if ( quad.readDevControl()){
      quad.controlAction();
    }
    

    quad.quadRegulator();
    quad.motorWrite();


}  
    
}   
