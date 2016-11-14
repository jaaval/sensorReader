
#include "sensorSampler.h"
#include <chrono>
#include <unistd.h>
#include <cassert>
#include <iostream>
//#include "algorithm/manager.hpp"


SensorSampler::SensorSampler(const char* i2c):
	imu(i2c)
{}


void SensorSampler::run() {
	
	uint64_t timesync = 1000000000; // 1 Hz dummy time sync. in this case the client and sensor times are really the same

	
	std::chrono::steady_clock::time_point synctime = std::chrono::steady_clock::now();

	IMU::Output out; // struct for storing sensor values
	//cpa::algorithm::manager man;
	//man.defineAvailableSensors(cpa::api::sensorAvailability::AVAILABLE_UNCALIBRATED,
	//	cpa::api::sensorAvailability::AVAILABLE_UNCALIBRATED,
	//	cpa::api::sensorAvailability::AVAILABLE_UNCALIBRATED,
	//	cpa::api::sensorAvailability::AVAILABLE_NONE);
	//man.setTime(synctime,synctime);


	int i = 0;
	while (true) {
		imu.read(out);
		//if (std::chrono::duration_cast<std::chrono::nanoseconds>(out.magTime-magtime).count() >= magdt) { 
		if (out.newMag) {
			//man.addSample(cpa::api::imuSensor::IMU_MAGNETOMETER, out.magTime, out.magValues[0], out.magValues[1], out.magValues[2]);
			out.newMag = false;
			std::cout << out.magTime<<" "<<out.magValues[0]<<" "<<out.magValues[1]<<" "<<out.magValues[2]<<std::endl;
		}

		if (out.newGyro) {
			//man.addSample(cpa::api::imuSensor::IMU_GYROSCOPE, out.gyroTime, out.gyroValues[0], out.gyroValues[1], out.gyroValues[2]);
			std::cout << out.gyroTime<<" "<<out.gyroValues[0]<<" "<<out.gyroValues[1]<<" "<<out.gyroValues[2]<<std::endl;
			out.newGyro = false;
		}

		if (out.newAcc) {
			//man.addSample(cpa::api::imuSensor::IMU_ACCELEROMETER, out.accTime, out.accValues[0], out.accValues[1], out.accValues[2]);
			std::cout << out.accTime<<" "<<out.accValues[0]<<" "<<out.accValues[1]<<" "<<out.accValues[2]<<std::endl;
			out.newAcc = false;
		}


		// push to cpa or something
		i++;
	}

}