#ifndef easyPID_H_
#define easyPID_H_

#include <Arduino.h>
#define AUTOMATIC 1
#define MANUAL 0


class easyPID
{
public:
	easyPID(int*, int* , int*, float, float, float, int, int, unsigned long);

	void compute();

	void setParameters(float, float, float);

	void setMode(int);

	void init();

private:
	int *pidInput;
        int *pidOutput;
        int *pidSetpoint;
        int lastInput;
        int pidOutMax;
        int pidOutMin;
        float ki, kd, kp;
        float iTerm;
        unsigned long sampleTime;
        bool inAuto;

};



#endif
