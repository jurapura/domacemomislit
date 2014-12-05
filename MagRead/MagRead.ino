#include <Wire.h>
#include "defines.h"

//###############################
//###       Main program      ###
//###############################

void setup()
{
  Wire.begin();
  //initSensor(); 
  testSensor();
  Serial.begin(9600);
}

void loop()
{
  readSensor1();
  delay(333);
}
