
#include "sensors.h"
#include <iostream>
#include <chrono>
#include "Eigen/Core"


void run(IMU &imu) {

	auto time = std::chrono::high_resolution_clock::now();
	Eigen::Vector3f samplevec;

	int i = 0;
	while (i < 1000) {
		time = std::chrono::high_resolution_clock::now();

		samplevec = imu.readAcc();
		std::cout << "Acc: " << std::chrono::duration_cast<Sec>(time) << " " << samplevec << std::endl;
		samplevec = imu.readMag();
		std::cout << "Mag: " << std::chrono::duration_cast<Sec>(time) << " " << samplevec << std::endl;
		samplevec = imu.readGyro();
		std::cout << "Gyro: " << std::chrono::duration_cast<Sec>(time) << " " << samplevec << std::endl;
		i++;
	}

}


int main() {

	std::string i2cdevice = "/dev/i2c-0";
	Sensors imu(i2cdevice.c_str());
	run(imu);
	return 0;
}
