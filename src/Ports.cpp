/*
 * Ports.cpp
 *
 *  Created on: 7.1.2012
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#include <list>
#include "PortDevice.h"

#include "Ports.h"

Ports::Ports() {
	// TODO Auto-generated constructor stub
}

Ports::~Ports() {
	// TODO Auto-generated destructor stub
}

uint8_t Ports::pread(uint16_t port)
{
	uint8_t value = 0xff; // pull-up

	std::list<PortDevice*>::iterator device;
	for (device = devices.begin(); device != devices.end(); device++) {
		(*device)->pread(port, &value);
	}

	return value;
}

void Ports::connectDevice(PortDevice *device) {
	devices.push_back(device);
}

void Ports::removeDevice(PortDevice *device) {
	devices.remove(device);
}

void Ports::pwrite(uint16_t port, uint8_t value)
{
	std::list<PortDevice*>::iterator device;
	for (device = devices.begin(); device != devices.end(); device++) {
		(*device)->pwrite(port, value);
	}
}



