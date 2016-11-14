
#include "sensordevices.h"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <climits>
#include <algorithm>

#define PI 3.1415926535897


Altimu10v5::Altimu10v5(const char * i2cDeviceName) :
  compass(i2cDeviceName), gyroacc(i2cDeviceName)
{
    // This likely depends on the sensor settings
    gyro_bias[0] = -45;
    gyro_bias[1] = -102;
    gyro_bias[2] = -160;          

    // scaling values are dependent on the sensor settings 
    mag_scale = 12 *100.0/32768.0;                   // 16bit max value is 12 gauss
    acc_scale = 2 *9.80665/32768.0;                  // 16bit max value is 2g
    gyro_scale = 1000.0 /360.0 *(2*PI) /32768.0;     // 16bit max value is 1000 deg/s
    time0 = std::chrono::steady_clock::now();        // time0 is reinitialized when the clock runs over - happens a lot with small systems like raspi
    lasttime = std::chrono::steady_clock::now();
    magAvailable = true;
    gyroAvailable = true;
    accAvailable = true;
    baroAvailable = false;
    enable();
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
    if (!compass.read()) return;

    out.magTime = readTime();
    std::copy(std::begin(compass.m),std::end(compass.m), out.rawMag); 
    for (int i = 0; i < 3; i++) {
        out.magValues[i] = compass.m[i]*mag_scale;
    }
}

void Altimu10v5::readAcc(Output &out)
{
    if (!gyroacc.readAcc()) return;

    out.accTime = readTime();
    std::copy(std::begin(gyroacc.a),std::end(gyroacc.a), out.rawAcc);
    for (int i = 0; i < 3; i++) {
        out.accValues[i] = gyroacc.a[i]*acc_scale;
    }
}

void Altimu10v5::readGyro(Output &out)
{
    if (!gyroacc.readGyro()) return;

    out.gyroTime = readTime();
    std::copy(std::begin(gyroacc.g),std::end(gyroacc.g), out.rawGyro);
    for (int i = 0; i < 3; i++) {
        out.gyroValues[i] = (gyroacc.g[i]-gyro_bias[i])*acc_scale;
    }
}

// not implemented yet. No sensorchip class for barometer yet.
void Altimu10v5::readBaro(Output &out) {
    out.rawBaro[0] = 0;
    out.baroTime = readTime();
    out.baroValues[0] = 0;
}

void Altimu10v5::read(Output &out) {
    if (magAvailable) readMag(out);
    if (gyroAvailable) readGyro(out);
    if (accAvailable) readAcc(out);
    if (baroAvailable) readBaro(out);
}

float Altimu10v5::readTime()
{
    lasttime = std::chrono::steady_clock::now();
    uint64_t temp = std::chrono::duration_cast<std::chrono::nanoseconds>(lasttime-time0).count();
    if (temp < 0) {
        time0 = lasttime;
        temp = 0;
    }
    temp;
}
