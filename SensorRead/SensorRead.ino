#include <Wire.h>
#include "config.h"

//###############################
//###       Main program      ###
//###############################

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  initSensors(); 
}

void loop()
{
  readSensor();
  delay(333);
}
