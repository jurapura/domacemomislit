/**
 * SensorFusion.h
 * 
 * Implementation of Madgwick's MARG algorithm.
 * 
 * Estimation of IMU and MARG orientation using a
 * gradient descent algorithm, 2011.
 * 
 */

#ifndef SensorFusion_H_
#define SensorFusion_H_

#include "MPU6050.h"
#include "HMC5883L.h"
#include <Arduino.h>

#define betaDef     0.4f    // 2 * gain
#define M_PI        3.14159265359f

class SensorFusion
{
public:
    SensorFusion();

    void init();
    
    void getQ(float* q);

    void getRaw(float* values);

    void ahrsUpdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
    
    void getSensorReadings(float* values);

    void getYawPitchRollRad(float* rpy);

    void getYawPitchRollDeg(float* rpy);
    
    void zeroGyro(int number);
    
    void getYawPitchRollDeg2(double* ypr);
    
    void getYawPitchRollDeg3(double* ypr);

    MPU6050 accelgyro;
    HMC5883L magn;
    
    float gE[3];

private:
    float invSqrt(float x);

    float beta;
    float q0, q1, q2, q3;


    float sampleFreq;

    unsigned long lastUpdate, now;

};

#endif
