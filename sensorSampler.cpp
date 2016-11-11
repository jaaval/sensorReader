
#include "sensorSampler.h"
#include <chrono>
#include <unistd.h>
#include <iostream>


SensorSampler::SensorSampler(const char* i2c):
	imu(i2c),
	magsr(50),
	gyrosr(100),
	accsr(100)
{}


void SensorSampler::run() {
	float magdt = 1000.0/magsr;
	float gyrodt = 1000.0/gyrosr;
	float accdt = 1000.0/accsr;

	std::chrono::steady_clock::time_point gyrotime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point acctime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point magtime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();

	IMU::Output out;

	while (true) {
		imu.read(out);
		t = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(t-magtime).count() >= magdt) {
			magtime = t;
			std::cout << "Mag: " << out.magValues[0] << " " << out.magValues[1] << " " << out.magValues[2] << " " << out.magValues[3] << std::endl;
		}

		if (std::chrono::duration_cast<std::chrono::milliseconds>(t-gyrotime).count() >= gyrodt) {
			gyrotime = t;
			std::cout << "Acc: " << out.accValues[0] << " " << out.accValues[1] << " " << out.accValues[2] << " " << out.accValues[3] << std::endl;
		}

		if (std::chrono::duration_cast<std::chrono::milliseconds>(t-acctime).count() >= accdt) {
			acctime = t;
			std::cout << "Gyro: " << out.gyroValues[0] << " " << out.gyroValues[1] << " " << out.gyroValues[2] << " " << out.gyroValues[3] << std::endl;
		}
		//usleep(1000); 

		// push to cpa or something

	}

}