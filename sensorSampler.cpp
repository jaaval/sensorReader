
#include "sensorSampler.h"
#include <chrono>
#include <unistd.h>
#include <cassert>
#include <iostream>


SensorSampler::SensorSampler(const char* i2c):
	imu(i2c),
	magsr(40),
	gyrosr(100),
	accsr(100)
{}


void SensorSampler::run() {
	float magdt = 1000000.0/magsr;
	float gyrodt = 1000000.0/gyrosr;
	float accdt = 1000000.0/accsr;

	std::chrono::steady_clock::time_point gyrotime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point acctime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point magtime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();

	IMU::Output out;
	//cpa::algorithm::manager man;
	//man.defineAvailableSensors(cpa::api::sensorAvailability::AVAILABLE_UNCALIBRATED,
	//	cpa::api::sensorAvailability::AVAILABLE_UNCALIBRATED,
	//	cpa::api::sensorAvailability::AVAILABLE_UNCALIBRATED,
	//	cpa::api::sensorAvailability::AVAILABLE_NONE);

	int i = 0;
	while (true) {
		imu.read(out);
		t = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::nanoseconds>(t-magtime).count() >= magdt) {
			magtime = t;
			//man.addSample(cpa::api::imuSensor::IMU_MAGNETOMETER, out.magTime, out.magValues[0], out.magValues[1], out.magValues[2]);
			std::cout << out.magTime<<" "<<out.magValues[0]<<" "<<out.magValues[1]<<" "<<out.magValues[2]<<std::endl;
		}

		if (std::chrono::duration_cast<std::chrono::nanoseconds>(t-gyrotime).count() >= gyrodt) {
			gyrotime = t;
			//man.addSample(cpa::api::imuSensor::IMU_GYROSCOPE, out.gyroTime, out.gyroValues[0], out.gyroValues[1], out.gyroValues[2]);
			std::cout << out.gyroTime<<" "<<out.gyroValues[0]<<" "<<out.gyroValues[1]<<" "<<out.gyroValues[2]<<std::endl;
		}

		if (std::chrono::duration_cast<std::chrono::nanoseconds>(t-acctime).count() >= accdt) {
			acctime = t;
			//man.addSample(cpa::api::imuSensor::IMU_ACCELEROMETER, out.accTime, out.accValues[0], out.accValues[1], out.accValues[2]);
			std::cout << out.accTime<<" "<<out.accValues[0]<<" "<<out.accValues[1]<<" "<<out.accValues[2]<<std::endl;
		}
		//usleep(1000); 

		// push to cpa or something
		i++;
	}

}