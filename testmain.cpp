
#include "sensordevices.h"
#include "sensorSampler.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <unistd.h>


int main() {

	std::string i2cdevice = "/dev/i2c-1";
	SensorSampler ss(i2cdevice.c_str());
	ss.run();
	return 0;
}
