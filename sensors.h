#ifndef _SENSORS_H
#define _SENSORS_H

#include "IMU.h"
#include "LSM6.h"
#include "LIS3MDL.h"

class Sensors : public IMU {
public:
    Sensors(const char * i2cDeviceName);

    LSM6 gyroacc;
    LIS3MDL compass;

    virtual Eigen::Vector3f readAcc();
    virtual Eigen::Vector3f readMag();
    virtual Eigen::Vector3f readGyro();
    virtual double readTime();
    virtual double readBaro();

    virtual void enable();
    virtual void measureOffsets();
};

#endif