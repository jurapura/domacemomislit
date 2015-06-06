#include "Arduino.h"
#include <EEPROM.h>
#include <Servo.h>
#include "SensorFusion.h"



#define EEPROM_gyroBias_X_addr 0
#define EEPROM_gyroBias_Y_addr 4
#define EEPROM_gyroBias_Z_addr 8
#define EEPROM_rpKp_addr 12
#define EEPROM_rpKi_addr 16
#define EEPROM_rpKd_addr 20
#define EEPROM_yKp_addr 24
#define EEPROM_yKi_addr 28
#define EEPROM_yKd_addr 32
#define EEPROM_drpKp_addr 36
#define EEPROM_drpKi_addr 40
#define EEPROM_drpKd_addr 44
#define EEPROM_dyKp_addr 48
#define EEPROM_dyKi_addr 52
#define EEPROM_dyKd_addr 56
#define EEPROM_throtK_addr 60

#define int16_to_float_div 100.0
#define commSer Serial1
#define maxMessageLength 30
#define normalModeLength 11

#define MOTOR_FL 5
#define MOTOR_FR 10
#define MOTOR_BL 9
#define MOTOR_BR 12
#define motorMinOut 1072
#define motorMaxOut 1860
#define refToAng 50   //scale interval [-500,500] to degrees



class quadControl 
{
public:
	quadControl();
	
	void init();
	
	bool readDevControl();
	
	void controlAction();

	void attiutudeEstimation();
	
	void quadRegulator();
	
	void motorWrite();

        void motorMix(int,int,int,int, double, double);

        void ESCcalibration();
	
	void writeDevControl();
    

private:
    uint8_t messageBuffer[maxMessageLength];
    uint8_t bytesInBuffer;
   
    uint16_t refRoll;
    uint16_t refPitch;
    uint16_t refYaw;
    uint16_t refThrottle;
    double refRollAng,refPitchAng,refYawAng;
    Servo motorFL;
    Servo motorFR;
    Servo motorBL;
    Servo motorBR;
    SensorFusion marg;
    double ypr[3] = {0, 0, 0};
    bool Armed;
    
    float rpKp,rpKi,rpKd,yKp,yKi,yKd;
    float drpKp,drpKi,drpKd,dyKp,dyKi,dyKd;
    float throtK;
    
    int outRollAngle,outPitchAngle,outYawAngle,outThrottle;

    
};


