/* Library for quadrotor control 

*/
#include "quadControl.h"

quadControl::quadControl()
{
	
}

void quadControl::init()
	{
		
		//initialization of communication
		bytesInBuffer=0;
		commInProces=False;
		
		//read fomr EEPROM		
		EEPROM.get(gyro_bias[0],EEPROM_gyroBias_X_addr);
		EEPROM.get(gyro_bias[1],EEPROM_gyroBias_Y_addr);
		EEPROM.get(gyro_bias[2],EEPROM_gyroBias_Z_addr);
		for (int i=0;i<3;i++)
		{
			gyro_bias[i]/=int16_to_float_div;
		}
		
	}
	
bool quadControl::readDevControl()
	{
		uint8_t inByte;
		inByte="0";
		if (commSer.available() > 0) 
		{
    		while(commSer.available()!=0 && inByte!="!")
    		{
    			inByte = Serial.read();
    			
    			if (bytesInBuffer>0){    			
    				messageBuffer[bytesInBuffer]=inByte;
    				bytesInBuffer+=1;
    			}
    			else {
    				if (inByte=="$"){
    					messageBuffer[bytesInBuffer]=inByte;
    					bytesInBuffer+=1;
    				}
    				else inByte="0";
    			}
    		}
    		
    		if (messageBuffer[0]=="$" && messageBuffer[bytesInBuffer]=="!"){
    			return True
    		}
    		else{
    			bytesInBuffer=0;
    			return False
    		}
    	}
    	else return False;   
             
    }
    
void quadControl::controlAction(){
	switch(messageBuffer[1]){
		case 1:      //gyro bias calibration
			///////// UMETNI POZIV KALIBRACIJE
			break;
		case 10:    //quadrotor arm
			///////// UMETNI POZIV ARMANJA
			break;
		case 20:    //normal mode
			///////// UMETNI 
			break;
			
			
			
	}	
}





