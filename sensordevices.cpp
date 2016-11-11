
#include "sensordevices.h"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <climits>

#define PI 3.1415926535897


Altimu10v5::Altimu10v5(const char * i2cDeviceName) :
  compass(i2cDeviceName), gyroacc(i2cDeviceName)
{
    // This likely depends on the sensor settings
    gyro_bias = {-45,-102,-160};          

    // scaling values are dependent on the sensor settings 
    mag_scale = 12 *100.0/32767.0;                   // 16bit max value is 12 gauss
    acc_scale = 2 *9.80665/32767.0;                  // 16bit max value is 2g
    gyro_scale = 1000.0 /360.0 *(2*PI) /32767.0;     // 16bit max value is 1000 deg/s
    time0 = std::chrono::steady_clock::now();        // time0 is reinitialized when the clock runs over - happens a lot with small systems like raspi
    lasttime = std::chrono::steady_clock::now();
    magAvailable = true;
    gyroAvailable = true;
    accAvailable = true;
    baroAvailable = false;
}

void Altimu10v5::enable()
{
    compass.enable();
    gyroacc.enable();
}

// can implement gyro calibration procedure here
void Altimu10v5::measureOffsets()
{
    
}

void Altimu10v5::readMag(Output &out)
{
    compass.read();
    float t = readTime();
    out.rawMag = {t, compass.m[0],compass.m[1],compass.m[2]};
    out.magValues = {t, compass.m[0]*mag_scale, compass.m[1]*mag_scale, compass.m[2]*mag_scale}; 
}

void Altimu10v5::readAcc(Output &out)
{
    gyroacc.readAcc();
    gyroacc.readTime();
    float t = readTime();
    out.rawAcc = {t, gyroacc.a[0], gyroacc.a[1], gyroacc.a[2]};
    out.accValues = {t, gyroacc.a[0]*acc_scale, gyroacc.a[1]*acc_scale, gyroacc.a[2]*acc_scale}; 
}

void Altimu10v5::readGyro(Output &out)
{
    gyroacc.readGyro();
    gyroacc.readTime();
    float t = readTime();
    out.rawGyro = {t, gyroacc.g[0], gyroacc.g[1], gyroacc.g[2]};
    out.gyroValues = {t, (gyroacc.g[0]-gyro_scale[0])*gyro_scale, (gyroacc.g[1]-gyro_scale[1])*gyro_scale, (gyroacc.g[2]-gyro_scale[2])*gyro_scale}; 
}

// not implemented yet. No sensorchip class for barometer yet.
void Altimu10v5::readBaro(Output &out) {
    float t = readTime();
    out.rawBaro = {t, 0};
    out.baroValues = {t, 0};
}

float Altimu10v5::readTime()
{
    lasttime = std::chrono::steady_clock::now();
    long temp = std::chrono::duration_cast<std::chrono::milliseconds>(lasttime-time0).count();
    if (temp < 0) {
        time0 = lasttime;
        temp = 0;
    }
    return 1e-3 * (temp);
}
