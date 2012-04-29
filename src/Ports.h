// Ports.h

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

#ifndef PORTS_H_
#define PORTS_H_

#include <list>
#include "PortDevice.h"

class Ports {
public:
	Ports();
	virtual ~Ports();

	void connectDevice(PortDevice *device);
	void removeDevice(PortDevice *device);

	uint8_t pread(uint16_t port);
	void pwrite(uint16_t port, uint8_t value);

private:
	std::list<PortDevice*> devices;
};

#endif /* PORTS_H_ */
