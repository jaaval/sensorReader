#include "vector.h"
#include "sensors.h"
#include "exceptions.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <wordexp.h>

Sensors::Sensors(const char * i2cDeviceName) :
  compass(i2cDeviceName), gyroacc(i2cDeviceName)
{
}

void Sensors::enable()
{
    compass.enable();
    gyroacc.enable();
}


void Sensors::measureOffsets()
{
    /*
    // TODO scales
    gyro_offset = vector::Zero();
    const int sampleCount = 32;
    for(int i = 0; i < sampleCount; i++)
    {
        gyroacc.read();
        gyro_offset += vector_from_ints(gyroacc.g);
        usleep(20*1000);
    }
    gyro_offset /= sampleCount;
    */
}

vector Sensors::readMag()
{
    compass.read();
    IMU::raw_m << compass.m[0], compass.m[1], compass.m[2];
    
    vector v;
    v(0) = (float)(compass.m[0]);
    v(1) = (float)(compass.m[1]);
    v(2) = (float)(compass.m[2]);
    return v;
}

vector Sensors::readAcc()
{
    // Info about linear acceleration sensitivity from datasheets:
    //TODO
    const float accel_scale = 1;

    gyroacc.readAcc();
    IMU::raw_a << gyroacc.a[0], gyroacc.a[1], gyroacc.a[2];
    vector v;
    v(0) = (float)(gyroacc.a[0]);
    v(1) = (float)(gyroacc.a[1]);
    v(2) = (float)(gyroacc.a[2]);
    return v;
}

vector Sensors::readGyro()
{
    // Info about sensitivity from datasheets:
    // TODO
    const float gyro_scale = 1;

    gyroacc.readGyro();
    IMU::raw_g << gyroacc.g[0], gyroacc.g[1], gyroacc.g[2];
    vector v;
    v(0) = (float)(gyroacc.g[0]);
    v(1) = (float)(gyroacc.g[1]);
    v(2) = (float)(gyroacc.g[2]);
    return v;
}
