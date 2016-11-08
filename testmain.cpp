
#include "sensors.h"
#include <iostream>
#include <chrono>
#include "Eigen/Core"
#include <cstdlib>


void run(IMU &imu) {

	std::time_t time0 = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::time_t time = time0;
	Eigen::Vector3f samplevec;

	int i = 0;
	while (i < 1000) {
		time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - time0;
		samplevec = imu.readAcc();
		std::cout << "Acc: " << time << " " << samplevec << std::endl;
		samplevec = imu.readMag();
		std::cout << "Mag: " << time << " " << samplevec << std::endl;
		samplevec = imu.readGyro();
		std::cout << "Gyro: " << time << " " << samplevec << std::endl;
		sleep(0.5); 


		i++;
	}

}


int main() {

	std::string i2cdevice = "/dev/i2c-1";
	Sensors imu(i2cdevice.c_str());
	run(imu);
	return 0;
}
