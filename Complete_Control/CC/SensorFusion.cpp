/**
 * SensorFusion.cpp
 * 
 * Implementation of Madgwick's MARG algorithm.
 * 
 * Estimation of IMU and MARG orientation using a
 * gradient descent algorithm, 2011.
 * 
 */

#include "SensorFusion.h"

/** Default constructor.
 */
SensorFusion::SensorFusion()
{
    accelgyro = MPU6050();

    magn = HMC5883L();
        
    beta = betaDef;
    q0 = 1.0f; q1 = 0.0f; q2 = 0.0f; q3 = 0.0f;
    lastUpdate = 0;
    now = 0;
}

/**
 * @brief SensorFusion initialization
 * @details
 * Initiates the accelerometer and gyroscope, as well as magnetometer.
 * Sets default values of +/-250 deg/s range for gyro, +/- 2g for accel and 1.3 Ga 
 * for magn. Also sets the Continuous Measurement Mode for magn and 75Hz refresh rate.
 */
void SensorFusion::init()
{
    accelgyro.init();
    
    accelgyro.setDLPF(4);
    accelgyro.setGyroConfig(3);
    accelgyro.setAccelConfig(2);

    magn.init();
    magn.setContMode();
    magn.setDataOutRate(6);
}

void SensorFusion::getQ(float* q)
{
    float values[9];
    getSensorReadings(values);

    now = micros();
    sampleFreq = 1.0 / ((now - lastUpdate) / 1000000.0f);
    lastUpdate = now;

    ahrsUpdate(values[0] * M_PI/180, values[1] * M_PI/180, values[2] * M_PI/180, values[3], values[4], values[5], values[6], values[7], values[8]);

   // Serial.print("val1:\t");
   // Serial.print(q0); Serial.print("   val2:\t");
   // Serial.print(q1); Serial.print("   val3:\t");
   // Serial.print(q2); Serial.print("   val4:\t");
   // Serial.println(q3);

    q[0] = q0;
    q[1] = q1;
    q[2] = q2;
    q[3] = q3;
}
    
void SensorFusion::getSensorReadings(float* values)
{
    int16_t vals[6];
    accelgyro.getMotion6(&vals[0], &vals[1], &vals[2], &vals[3], &vals[4], &vals[5]);

    for (int i = 0; i < 6; i++)
    {
        if(i < 3)
        {
            values[i+3] = (float) vals[i];
        }
        else
        {
            values[i-3] = ((float) vals[i] - gE[i-3]) / 16.4f;
        }
    }

    magn.getRaw(&vals[0], &vals[1], &vals[2]);

    for (int i = 0; i < 3; i++)
    {
        values[6+i] = (float) vals[i];
    }
}

void SensorFusion::ahrsUpdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
{
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float hx, hy;
    float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _8bx, _8bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;


    // rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);


    // Normalise accelerometer measurement
    recipNorm = invSqrt(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;
    

    // Normalise magnetometer measurement
    recipNorm = invSqrt(mx * mx + my * my + mz * mz);
    mx *= recipNorm;
    my *= recipNorm;
    mz *= recipNorm;


    // Auxiliary variables to avoid repeated arithmetic
    _2q0mx = 2.0f * q0 * mx;
    _2q0my = 2.0f * q0 * my;
    _2q0mz = 2.0f * q0 * mz;
    _2q1mx = 2.0f * q1 * mx;
    _2q0 = 2.0f * q0;
    _2q1 = 2.0f * q1;
    _2q2 = 2.0f * q2;
    _2q3 = 2.0f * q3;
    _2q0q2 = 2.0f * q0 * q2;
    _2q2q3 = 2.0f * q2 * q3;
    q0q0 = q0 * q0;
    q0q1 = q0 * q1;
    q0q2 = q0 * q2;
    q0q3 = q0 * q3;
    q1q1 = q1 * q1;
    q1q2 = q1 * q2;
    q1q3 = q1 * q3;
    q2q2 = q2 * q2;
    q2q3 = q2 * q3;
    q3q3 = q3 * q3;


    // Reference direction of Earth's magnetic field
    hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
    hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
    _2bx = sqrt(hx * hx + hy * hy);
    _2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
    _4bx = 2.0f * _2bx;
    _4bz = 2.0f * _2bz;
    _8bx = 2.0f * _4bx;
    _8bz = 2.0f * _4bz;
    
//    Serial.print("val1:\t");
//    Serial.print(hx); Serial.print("   val2:\t");
//    Serial.print(hy); Serial.print("   val3:\t");
//    Serial.print(_2bx); Serial.print("   val4:\t");
//    Serial.println(_2bz);


    // Gradient decent algorithm corrective step
    s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _4bz * q2 * (_4bx * (0.5f - q2q2 - q3q3) + _4bz * (q1q3 - q0q2) - mx) + (-_4bx * q3 + _4bz * q1) * (_4bx * (q1q2 - q0q3) + _4bz * (q0q1 + q2q3) - my) + _4bx * q2 * (_4bx * (q0q2 + q1q3) + _4bz * (0.5f - q1q1 - q2q2) - mz);
    s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _4bz * q3 * (_4bx * (0.5f - q2q2 - q3q3) + _4bz * (q1q3 - q0q2) - mx) + (_4bx * q2 + _4bz * q0) * (_4bx * (q1q2 - q0q3) + _4bz * (q0q1 + q2q3) - my) + (_4bx * q3 - _8bz * q1) * (_4bx * (q0q2 + q1q3) + _4bz * (0.5f - q1q1 - q2q2) - mz);
    s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_8bx * q2 - _4bz * q0) * (_4bx * (0.5f - q2q2 - q3q3) + _4bz * (q1q3 - q0q2) - mx) + (_4bx * q1 + _4bz * q3) * (_4bx * (q1q2 - q0q3) + _4bz * (q0q1 + q2q3) - my) + (_4bx * q0 - _8bz * q2) * (_4bx * (q0q2 + q1q3) + _4bz * (0.5f - q1q1 - q2q2) - mz);
    s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_8bx * q3 + _4bz * q1) * (_4bx * (0.5f - q2q2 - q3q3) + _4bz * (q1q3 - q0q2) - mx) + (-_4bx * q0 + _4bz * q2) * (_4bx * (q1q2 - q0q3) + _4bz * (q0q1 + q2q3) - my) + _4bx * q1 * (_4bx * (q0q2 + q1q3) + _4bz * (0.5f - q1q1 - q2q2) - mz);


    // normalise step magnitude
    recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
    
    s0 *= recipNorm;
    s1 *= recipNorm;
    s2 *= recipNorm;
    s3 *= recipNorm;


    // Apply feedback step
    qDot1 -= beta * s0;
    qDot2 -= beta * s1;
    qDot3 -= beta * s2;
    qDot4 -= beta * s3;


//    // Calculate sampleFreq
//    now = micros();
//    sampleFreq = 1.0 / ((now - lastUpdate) / 1000000.0);
//    lastUpdate = now;


    // Integrate rate of change of quaternion to yield quaternion
    q0 += qDot1 * (1.0f / sampleFreq);
    q1 += qDot2 * (1.0f / sampleFreq);
    q2 += qDot3 * (1.0f / sampleFreq);
    q3 += qDot4 * (1.0f / sampleFreq);
    
//    Serial.print("val1:\t");
//    Serial.print(q0); Serial.print("   val2:\t");
//    Serial.print(q1); Serial.print("   val3:\t");
//    Serial.print(q2); Serial.print("   val4:\t");
//    Serial.println(q3);
//    Serial.println();
    delay(1);

//    Serial.print("val1:\t");
//    Serial.print(qDot1); Serial.print("   val2:\t");
//    Serial.print(qDot2); Serial.print("   val3:\t");
//    Serial.print(qDot3); Serial.print("   val4:\t");
//    Serial.println(qDot4);
//    Serial.println();


    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;
    
//    Serial.print("val1:\t");
//    Serial.print(q0); Serial.print("   val2:\t");
//    Serial.print(q1); Serial.print("   val3:\t");
//    Serial.print(q2); Serial.print("   val4:\t");
//    Serial.println(q3);
//    Serial.println();
}

void SensorFusion::getYawPitchRollRad(float * ypr) 
{
  float q[4]; // quaternion
  float gx, gy, gz; // estimated gravity direction
  getQ(q);
  
  gx = 2 * (q[1] * q[3] - q[0] * q[2]);
  gy = 2 * (q[0] * q[1] + q[2] * q[3]);
  gz = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
  
  ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0] * q[0] + 2 * q[1] * q[1] - 1);
  ypr[1] = atan(gx / sqrt(gy * gy + gz * gz));
  ypr[2] = atan(gy / sqrt(gx * gx + gz * gz));
}

void SensorFusion::getYawPitchRollDeg(float * ypr) 
{
  float q[4]; // quaternion
  float gx, gy, gz; // estimated gravity direction
  getQ(q);
  
  gx = 2 * (q[1] * q[3] - q[0] * q[2]);
  gy = 2 * (q[0] * q[1] + q[2] * q[3]);
  gz = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
  
  ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0] * q[0] + 2 * q[1] * q[1] - 1) * 180/M_PI;
  ypr[1] = atan(gx / sqrt(gy * gy + gz * gz)) * 180/M_PI;
  ypr[2] = atan(gy / sqrt(gx * gx + gz * gz)) * 180/M_PI;
}

void SensorFusion::getYawPitchRollDeg2(double* ypr) 
{
  float q[4]; // quaternion
  getQ(q);
  float q22 = q[2] * q[2];
  
  ypr[0] = atan2(2*q[2]*q[3] + 2*q[0]*q[1], 1 - 2*q22 - 2*q[1]*q[1] ) * 180/M_PI;
  ypr[1] = asin(2*q[0]*q[2] - 2*q[1]*q[3]) * 180/M_PI;
  ypr[2] = atan2(2*q[0]*q[3] + 2*q[1]*q[2], 1 - 2*q22 - 2*q[3]*q[3] ) * 180/M_PI;
}

void SensorFusion::getYawPitchRollDeg3(double* ypr)
{
  float q[4]; // quaternion
  float q02, q12, q22, q32, q_temp; // estimated gravity direction
  getQ(q);
  
  q02 = q[0] * q[0];
  q12 = q[1] * q[1];
  q22 = q[2] * q[2];
  q32 = q[3] * q[3];
  q_temp = 2*q[1]*q[3] + 2*q[0]*q[2];
  
  ypr[0] = -atan(2*(q[1]*q[3] + q[2]*q[0])/sqrt(1-(q_temp * q_temp))) * 180/M_PI;
  ypr[2] = atan2(2*q[2]*q[3] - 2*q[0]*q[1], -1 + 2*q32 + 2*q02) * 180/M_PI;
  ypr[1] = atan2(2*q[1]*q[2] - 2*q[0]*q[3], - 1 + 2*q12 + 2*q02) * 180/M_PI;
}

float SensorFusion::invSqrt(float x){
   uint32_t i = 0x5F1F1412 - (*(uint32_t*)&x >> 1);
   float tmp = *(float*)&i;
   return tmp * (1.69000231f - 0.714158168f * x * tmp * tmp);
}

void SensorFusion::zeroGyro(int number)
{
  float x = 0, y = 0, z = 0;
  int16_t vals[6];
  
  for (int i = 0; i < number; i++)
  {
      accelgyro.getMotion6(&vals[0], &vals[1], &vals[2], &vals[3], &vals[4], &vals[5]);
      x += (float) vals[3];
      y += (float) vals[4];
      z += (float) vals[5];
  }
  gE[0] = x/number;
  gE[1] = y/number;
  gE[2] = z/number;
}
