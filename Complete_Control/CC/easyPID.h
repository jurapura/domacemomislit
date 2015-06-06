#ifndef easyPID_H_
#define easyPID_H_

#include <Arduino.h>
#define AUTOMATIC 1
#define MANUAL 0


class easyPID
{
public:
	easyPID(int* input, int* output, int* setpoint, float kp, float ki, float kd, int outMax, int outMin, unsigned long sampleTime);

	void compute();

	void setParameters(float k_p, float k_i, float k_d);

	void setMode(int mode);

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
