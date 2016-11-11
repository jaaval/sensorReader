
#include "sensordevices.h"
#include "sensorSampler.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <unistd.h>


void run(IMU &imu) {

	float time;
	imu.enable();
	sleep(1); 
	int i = 0;
	IMU::Output out;
	while (i < 100000) {
		imu.read(out);
		std::cout << "Mag: " << out.magValues[0] << " " << out.magValues[1] << " " << out.magValues[2] << " " << out.magValues[3] << std::endl;
		std::cout << "Acc: " << out.accValues[0] << " " << out.accValues[1] << " " << out.accValues[2] << " " << out.accValues[3] << std::endl;
		std::cout << "Gyro: " << out.gyroValues[0] << " " << out.gyroValues[1] << " " << out.gyroValues[2] << " " << out.gyroValues[3] << std::endl;
		usleep(500000); 
		i++;
	}

}


int main() {

	std::string i2cdevice = "/dev/i2c-1";
	SensorSampler ss(i2cdevice.c_str());
	ss.run();
	return 0;
}
