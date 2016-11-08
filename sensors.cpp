#include "vector.h"
#include "Sensors.h"
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

void Sensors::loadCalibration()
{
    wordexp_t expansion_result;
    wordexp("~/.Sensors-ahrs-cal", &expansion_result, 0);

    std::ifstream file(expansion_result.we_wordv[0]);
    if (file.fail())
    {
        throw posix_error("Failed to open calibration file ~/.Sensors-ahrs-cal");
    }

    file >> mag_min(0) >> mag_max(0) >> mag_min(1) >> mag_max(1) >> mag_min(2) >> mag_max(2);
    if (file.fail() || file.bad())
    {
        throw std::runtime_error("Failed to parse calibration file ~/.Sensors-ahrs-cal");
    }
}

void Sensors::measureOffsets()
{
    
    // TODO scales
    gyro_offset = vector::Zero();
    const int sampleCount = 32;
    for(int i = 0; i < sampleCount; i++)
    {
        gyroacc.read();
        gyro_offset += vector_from_ints(&gyroacc.g);
        usleep(20*1000);
    }
    gyro_offset /= sampleCount;
}

vector Sensors::readMag()
{
    compass.readMag();
    IMU::raw_m = int_vector_from_ints(&compass.m);
    
    vector v;
    v(0) = (float)(compass.m[0] - mag_min(0)) / (mag_max(0) - mag_min(0)) * 2 - 1;
    v(1) = (float)(compass.m[1] - mag_min(1)) / (mag_max(1) - mag_min(1)) * 2 - 1;
    v(2) = (float)(compass.m[2] - mag_min(2)) / (mag_max(2) - mag_min(2)) * 2 - 1;
    return v;
}

vector Sensors::readAcc()
{
    // Info about linear acceleration sensitivity from datasheets:
    //TODO
    const float accel_scale = 0.000244;

    gyroacc.readAcc();
    IMU::raw_a = int_vector_from_ints(&gyroacc.a);
    return vector_from_ints(&gyroacc.a) * accel_scale;
}

vector Sensors::readGyro()
{
    // Info about sensitivity from datasheets:
    // TODO
    const float gyro_scale = 0.07 * 3.14159265 / 180;

    gyroacc.read();
    IMU::raw_g = int_vector_from_ints(&gyroacc.g);
    return ( vector_from_ints(&gyroacc.g) - gyro_offset ) * gyro_scale;
}
