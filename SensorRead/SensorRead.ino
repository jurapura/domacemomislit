#include <Wire.h>
#include "config.h"

//###############################
//###       Main program      ###
//###############################

void setup()
{
  Wire.begin();
  initSensor(); //novo
  //writeTo(0x68, 0x6B, 0);
  Serial.begin(9600);
}

void loop()
{
  readSensor();
  delay(333);
}
