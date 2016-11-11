#ifndef _IMU_H
#define _IMU_H

#include <chrono>
#include <vector>

/*
 *  The interface to read sensors
 *  @author jaava
 */


class IMU {
public:

    struct Output {
        Output() {
            magValues = {0,0,0,0};
            gyroValues = {0,0,0,0};
            accValues = {0,0,0,0};
            baroValues = {0,0};
            rawMag = {0,0,0};
            rawAcc = {0,0,0};
            rawGyro = {0,0,0};
            rawBaro = {0};
        }

        std::vector<float> magValues;                     // sensor output is defined as [timestamp, sensorvalues ...]
        std::vector<float> gyroValues;
        std::vector<float> accValues;
        std::vector<float> baroValues;
        std::vector<int> rawMag;
        std::vector<int> rawGyro;
        std::vector<int> rawAcc;
        std::vector<int> rawBaro;
    };


    // Scaled readings
    virtual void readMag(Output out) = 0;       // microteslas
    virtual void readAcc(Output out) = 0;       // m/s^2
    virtual void readGyro(Output out) = 0;      // rad/s
    virtual void readBaro(Output out) = 0;
    virtual float readTime() = 0;               // returns the timestamp from system clock

    virtual void measureOffsets() = 0;          // could be used to calibrate sensors but not implemented atm.
    virtual void enable() = 0;                  // this sets the sensor chip settings. Has to be run at some point in initialization.
    
    virtual void read(Output &out) = 0;         //reads all the sensors to output

    bool hasMag() {return magAvailable;}
    bool hasGyro() {return gyroAvailable;}
    bool hasAcc() {return accAvailable;}
    bool hasBaro() {return baroAvailable;}

protected:
    int gyro_bias[3];              // gyro offset from zero
    
    // scaling values for sensor readings. The raw values are generally 16bit integers. 
    // these depend on the sensor settings (i.e. full range setting)
    float gyro_scale;
    float mag_scale;
    float acc_scale; 
    float baro_scale;
    // system timestamps. Sensors do not necessarily have their own clock at all
    std::chrono::steady_clock::time_point time0;
    std::chrono::steady_clock::time_point lasttime;
    // availability of sensors on particular imu implementation
    bool magAvailable, gyroAvailable, accAvailable, baroAvailable;

};

#endif
