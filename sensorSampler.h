
#ifndef SENSORSAMPLER_H
#define SENSORSAMPLER_H

#include "IMU.h"

class SensorSampler {

public:

	SensorSampler(IMU &imu_);

	void run();


private:
	IMU imu;
	int magsr;
	int gyrosr;
	int accsr;


};


#endif