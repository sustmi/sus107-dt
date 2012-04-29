/*
 * Ports.h
 *
 *  Created on: 7.1.2012
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

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
