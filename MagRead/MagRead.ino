#include <Wire.h>
#include "defines.h"

//###############################
//###       Main program      ###
//###############################

void setup()
{
  Wire.begin();
  initSensor(); 
  Serial.begin(9600);
}

void loop()
{
  readSensor();
  delay(20);
}
