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

    void readAcc(Output &out);
    void readMag(Output &out);
    void readGyro(Output &out);
    void readBaro(Output &out);

    float readTime();

    void enable();
    void measureOffsets();

    void read(Output &out);

private:
    LSM6 gyroacc;
    LIS3MDL compass;

};

#endif