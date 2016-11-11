
#ifndef SENSORSAMPLER_H
#define SENSORSAMPLER_H

#include "sensor.hpp"

class SensorSampler() {

public:

	SensorSampler(imu_):
		imu(imu_),
		magsr(50),
		gyrosr(100),
		accsr(100)
	{}

	run();


private:
	IMU imu;
	int magsr;
	int gyrosr;
	int accsr;


};


#endif