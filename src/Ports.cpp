// Ports.cpp

// Copyright (C) 2012  Miroslav Sustek <sus107@vsb.cz>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <list>
#include "PortDevice.h"

#include "Ports.h"

Ports::Ports() {

}

Ports::~Ports() {
	devices.clear();
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



