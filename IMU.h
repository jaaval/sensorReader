#ifndef _IMU_H
#define _IMU_H

#include "Eigen/Core"
#include <chrono>

class IMU {
public:
    // Scaled readings
    virtual Eigen::Vector3f readMag() = 0;  // In body coords, scaled to -1..1 range
    virtual Eigen::Vector3f readAcc() = 0;  // In body coords, with units = g
    virtual Eigen::Vector3f readGyro() = 0; // In body coords, with units = rad/sec
    void read(){ readAcc(); readMag(); readGyro(); }

    virtual void measureOffsets() = 0;
    virtual void enable() = 0;

    int_vector raw_m, raw_a, raw_g;

protected:
    Eigen::Vector3i gyro_bias;
    float gyro_scale;
    float mag_scale;
    float acc_scale; 

    std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> timenow;

};

#endif
