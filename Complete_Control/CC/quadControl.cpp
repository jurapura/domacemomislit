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
		
                marg.init();
		//read from EEPROM		
		EEPROM.get(EEPROM_gyroBias_X_addr, marg.gE[0]);
		EEPROM.get(EEPROM_gyroBias_Y_addr, marg.gE[1]);
		EEPROM.get(EEPROM_gyroBias_Z_addr, marg.gE[2]);
                
                EEPROM.get(EEPROM_rpKp_addr,rpKp);
                EEPROM.get(EEPROM_rpKi_addr,rpKi);
                EEPROM.get(EEPROM_rpKd_addr,rpKd);
                EEPROM.get(EEPROM_yKp_addr,yKp);
                EEPROM.get(EEPROM_yKi_addr,yKi);
                EEPROM.get(EEPROM_yKd_addr,yKd);
                EEPROM.get(EEPROM_drpKp_addr,drpKp);
                EEPROM.get(EEPROM_drpKi_addr,drpKi);
                EEPROM.get(EEPROM_drpKd_addr,drpKd);
                EEPROM.get(EEPROM_dyKp_addr,dyKp);
                EEPROM.get(EEPROM_dyKi_addr,dyKi);
                EEPROM.get(EEPROM_dyKd_addr,dyKd);
                EEPROM.get(EEPROM_throtK_addr,throtK);
                
                Serial.println(rpKp);
                Serial.println(throtK);

		refRollAng=0.0;
    	        refPitchAng=0.0;
    	        refYawAng=0.0;
    	        refThrottle=1000;
                
    
                outRollAngle=0;
                outPitchAngle=0;
                outYawAngle=0;
                outThrottle=0;
    
                Armed=0;
		
                motorFL.attach(MOTOR_FL);
                motorFR.attach(MOTOR_FR);
                motorBL.attach(MOTOR_BL);
                motorBR.attach(MOTOR_BR);

	}
	
bool quadControl::readDevControl()
	{
		uint8_t inByte;
		inByte='0';
                
		if (commSer.available() > 0) 
		{
                
    		while(commSer.available()>0 && inByte!='!')
    		{
    			inByte = commSer.read();
    			
                        if (inByte=='$'){
    					messageBuffer[bytesInBuffer]=inByte;
    					bytesInBuffer=1;
    				}
    			else if (bytesInBuffer>0){    			
    				messageBuffer[bytesInBuffer]=inByte;
    				bytesInBuffer+=1;
    			}
    			else {    				
    				inByte='0';
    			      }
    		}
                
    		if (messageBuffer[0]=='$' && messageBuffer[bytesInBuffer-1]=='!'){                       
    			return 1;
    		}
    		else{
    			bytesInBuffer=0;
    			return 0;
    		}
    	}
    	else return 0;   
             
    }
    
void quadControl::controlAction(){
        
	switch(messageBuffer[1]){
		case '1':      //gyro bias calibration
			///////// UMETNI POZIV KALIBRACIJE
                       	Serial.println("Gyroscope calibration");
                        marg.zeroGyro(5000);
                        delay(2000);
                        Serial.print(marg.gE[0]);Serial.print("  ");
                        Serial.print(marg.gE[1]);Serial.print("  ");
                        Serial.println(marg.gE[2]);
                        EEPROM.put(EEPROM_gyroBias_X_addr,marg.gE[0]);
		        EEPROM.put(EEPROM_gyroBias_Y_addr,marg.gE[1]);
		        EEPROM.put(EEPROM_gyroBias_Z_addr,marg.gE[2]);

			break;
		case '2':    //quadrotor arm
			///////// UMETNI POZIV ARMANJA
			Serial.println("Arm");
                        Armed=1;
			break;
		case '3':    //normal mode
			///////// UMETNI 
			if (bytesInBuffer==normalModeLength){
				refRoll=(messageBuffer[2] << 8) | messageBuffer[3];
				refPitch=(messageBuffer[4] << 8) | messageBuffer[5];
				refYaw=(messageBuffer[6] << 8) | messageBuffer[7];
				refThrottle=(messageBuffer[8] << 8) | messageBuffer[9];	

                                refRollAng=(refRoll-1500)/refToAng;
    	                        refPitchAng=(refRoll-1500)/refToAng;
    	                        refYawAng=(refRoll-1500)/refToAng;
    	                        refThrottle=1000;
				Serial.println("Normal mode");
                                Serial.print(refRoll);
                                Serial.print(refPitch);
                                Serial.print(refYaw);
                                Serial.println(refThrottle);		
			}
			else {
				Serial.println("Error normal mode");
			}
			

			break;
		case '4':  //regulator parametrization
			///////// DODAJ PODESAVANJE REGULATORA
                        rpKp=((messageBuffer[2] << 8) | messageBuffer[3])/int16_to_float_div;
			rpKi=((messageBuffer[4] << 8) | messageBuffer[5])/int16_to_float_div;
			rpKd=((messageBuffer[6] << 8) | messageBuffer[7])/int16_to_float_div;
			yKp=((messageBuffer[8] << 8) | messageBuffer[9])/int16_to_float_div;
                        yKi=((messageBuffer[10] << 8) | messageBuffer[11])/int16_to_float_div;	
                        yKd=((messageBuffer[12] << 8) | messageBuffer[13])/int16_to_float_div;
                        
                        drpKp=((messageBuffer[14] << 8) | messageBuffer[15])/int16_to_float_div;
			drpKi=((messageBuffer[16] << 8) | messageBuffer[17])/int16_to_float_div;
			drpKd=((messageBuffer[18] << 8) | messageBuffer[19])/int16_to_float_div;
			dyKp=((messageBuffer[20] << 8) | messageBuffer[21])/int16_to_float_div;
                        dyKi=((messageBuffer[22] << 8) | messageBuffer[23])/int16_to_float_div;	
                        dyKd=((messageBuffer[24] << 8) | messageBuffer[25])/int16_to_float_div;
                        
                        throtK=((messageBuffer[26] << 8) | messageBuffer[27])/int16_to_float_div;
                        Serial.print(throtK);
                        	
	
                        
			break;
		case '5': //save regulator parameters to EEPROM
			///////// DODAJ spremanje u eeprom
                        EEPROM.put(EEPROM_rpKp_addr,rpKp);
                        EEPROM.put(EEPROM_rpKi_addr,rpKi);
                        EEPROM.put(EEPROM_rpKd_addr,rpKd);
                        EEPROM.put(EEPROM_yKp_addr,yKp);
                        EEPROM.put(EEPROM_yKi_addr,yKi);
                        EEPROM.put(EEPROM_yKd_addr,yKd);
                        EEPROM.put(EEPROM_drpKp_addr,drpKp);
                        EEPROM.put(EEPROM_drpKi_addr,drpKi);
                        EEPROM.put(EEPROM_drpKd_addr,drpKd);
                        EEPROM.put(EEPROM_dyKp_addr,dyKp);
                        EEPROM.put(EEPROM_dyKi_addr,dyKi);
                        EEPROM.put(EEPROM_dyKd_addr,dyKd);
                        EEPROM.put(EEPROM_throtK_addr,throtK);
                        break;
                default:
                  break;
                }
         bytesInBuffer=0;
        
}	

void quadControl::ESCcalibration(){
        
      motorFL.writeMicroseconds(1000);
      delay(1000);
      motorFR.writeMicroseconds(1000);
      delay(1000);
      motorBL.writeMicroseconds(1000);
      delay(1000);
      motorBR.writeMicroseconds(1000);
      delay(1000);

         


}


void quadControl::quadRegulator(){
      if (!Armed) return;
      marg.getYawPitchRollDeg2(ypr);
      Serial.print(ypr[0]);
      Serial.print("    ");
      Serial.print(ypr[1]);
      Serial.print("    ");
      Serial.println(ypr[2]);
      
      this->motorMix(outRollAngle,outPitchAngle,outYawAngle,outThrottle,ypr[0],ypr[1]);
  }

void quadControl::motorWrite(){
        
        if (refThrottle<1000) { refThrottle = 1000;}
        if (refThrottle>2000) { refThrottle = 2000;}
        motorFL.writeMicroseconds((int) refThrottle);
        
}

void quadControl::motorMix(int outRoll, int outPitch, int outYaw, int Throttle, double roll, double pitch){
      int FL,FR,BL,BR;
      float drag;
      drag=abs(sin(roll))+abs(sin(pitch));
      FL= outPitch  +  outRoll  -  outYaw  +  Throttle  +  drag;
      FR= outPitch  -  outRoll  +  outYaw  +  Throttle  +  drag;
      BL=-outPitch  +  outRoll  +  outYaw  +  Throttle  +  drag;
      BR=-outPitch  -  outRoll  -  outYaw  +  Throttle  +  drag;
      constrain(FL,motorMinOut,motorMaxOut);
      constrain(FR,motorMinOut,motorMaxOut);
      constrain(BL,motorMinOut,motorMaxOut);
      constrain(BR,motorMinOut,motorMaxOut);
      
      motorFL.writeMicroseconds(FL);
      motorFR.writeMicroseconds(FR);
      motorBL.writeMicroseconds(BL);
      motorBR.writeMicroseconds(BR);
      Serial.print(FL);Serial.print("    ");Serial.println(FR);
      Serial.print(BL);Serial.print("    ");Serial.println(BR);
      Serial.println();
      
      
    

}




