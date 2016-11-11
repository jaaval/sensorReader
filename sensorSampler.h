
#ifndef SENSORSAMPLER_H
#define SENSORSAMPLER_H

#include "IMU.h"

class SensorSampler {

public:

	SensorSampler(const char* i2c);

	void run();


private:
	Altimu10v5 imu;
	int magsr;
	int gyrosr;
	int accsr;
};


#endif