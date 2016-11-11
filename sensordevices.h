#ifndef _SENSOR_DEVICES_H
#define _SENSOR_DEVICES_H

#include "IMU.h"
#include "LSM6.h"
#include "LIS3MDL.h"

/**
 * classes in this file implement IMU interface for different sensor configurations
 * @author jaava
 */


class Altimu10v5 : public IMU {
public:
    Altimu10v5(const char * i2cDeviceName);


    virtual void readAcc(Output &out);
    virtual void readMag(Output &out);
    virtual void readGyro(Output &out);
    virtual void readBaro(Output &out);

    virtual float readTime();

    virtual void enable();
    virtual void measureOffsets();

private:
    LSM6 gyroacc;
    LIS3MDL compass;

};

#endif