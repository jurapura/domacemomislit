/* PID control class */

#include "easyPID.h"

/** Default constructor
 */
 
easyPID::easyPID(int* input, int* output, int* setpoint, float kp, float ki, float kd, int outMax, int outMin, unsigned long sampleTime)
{
    inAuto = false;

    pidOutput = output;
    pidInput = input;
    pidSetpoint = setpoint;
    pidOutMax = outMax;
    pidOutMin = outMin;
    this->sampleTime = sampleTime;

    easyPID::setParameters(kp, ki, kd);
}

void easyPID::compute()
{
    if (!inAuto) return;

    int error = *pidSetpoint - *pidInput;
    iTerm += (ki * error);

    constrain(iTerm, pidOutMin, pidOutMax);

    int dInput = *pidInput - lastInput;

    *pidOutput = kp * error + iTerm - kd * dInput;

    constrain(*pidOutput, pidOutMin, pidOutMax);

    lastInput = *pidInput;

    return;

}

void easyPID::setParameters(float k_p, float k_i, float k_d)
{
    if (kp < 0 || ki < 0 || kd < 0) return;

    kp = k_p;
    ki = k_i * sampleTime/1000;
    kd = k_d / sampleTime/1000;
}

void easyPID::setMode(int mode)
{
    bool newAuto = (mode == AUTOMATIC);
    if(newAuto == !inAuto) easyPID::init();
    inAuto = newAuto;
}

void easyPID::init()
{
    iTerm = *pidOutput;
    lastInput = *pidInput;
    constrain(iTerm, pidOutMin, pidOutMax);
}
