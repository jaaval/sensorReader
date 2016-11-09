
#include "sensors.h"
#include <iostream>
#include <chrono>
#include "Eigen/Core"
#include <cstdlib>
#include <unistd.h>


void run(IMU &imu) {

	Eigen::Vector3f samplevec;
	double time;
	imu.enable();
	sleep(1); 
	int i = 0;
	while (i < 100000) {
		samplevec = imu.readMag();
		time = imu.readTime();
		std::cout << "Mag: " << time << " " << samplevec(0) << " " << samplevec(1) << " " << samplevec(2) << std::endl;
		samplevec = imu.readAcc();
		time = imu.readTime();
		std::cout << "Acc: " << time << " " << samplevec(0) << " " << samplevec(1) << " " << samplevec(2) << std::endl;
		samplevec = imu.readGyro();
		time = imu.readTime();
		std::cout << "Gyro: " << time << " " << samplevec(0) << " " << samplevec(1) << " " << samplevec(2) << std::endl;
		usleep(200000); 


		i++;
	}

}


int main() {

	std::string i2cdevice = "/dev/i2c-1";
	Sensors imu(i2cdevice.c_str());
	run(imu);
	return 0;
}
