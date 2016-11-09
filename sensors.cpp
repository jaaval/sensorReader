#include "vector.h"
#include "sensors.h"
#include "exceptions.h"
#include <unistd.h>
#include <iostream>
#include <fstream>

#define PI 3.1415926535897


Sensors::Sensors(const char * i2cDeviceName) :
  compass(i2cDeviceName), gyroacc(i2cDeviceName)
{
    gyro_bias << -45, -102, -160;                   // This likely depends on the sensor settings.
    mag_scale = 1200.0/32767.0;                     // scaling values are dependent on the sensor settings
    acc_scale = 2*9.80665/32767.0;                // scaling values are dependent on the sensor settings
    gyro_scale = 1000.0/360.0 *(2*PI) /32767.0;     // scaling values are dependent on the sensor settings
}

void Sensors::enable()
{
    compass.enable();
    gyroacc.enable();
}

// can implement gyro calibration procedure here
void Sensors::measureOffsets()
{
    
}

vector Sensors::readMag()
{
    compass.read();
    raw_m << compass.m[0], compass.m[1], compass.m[2];
    Eigen::Vector3f v = (raw_m).cast<float>() * mag_scale;
    return v;
}

vector Sensors::readAcc()
{
    gyroacc.readAcc();
    raw_a << gyroacc.a[0], gyroacc.a[1], gyroacc.a[2];
    Eigen::Vector3f v = (raw_a).cast<float>() * acc_scale;
    return v;
}

vector Sensors::readGyro()
{
    gyroacc.readGyro();
    raw_g << gyroacc.g[0], gyroacc.g[1], gyroacc.g[2];
    Eigen::Vector3f v = (raw_g - gyro_bias).cast<float>() * gyro_scale;
    return v;
}
