/*
 * PortDevice.h
 *
 *  Created on: 7.1.2012
 *      Author: mirek
 */

#ifndef PORTDEVICE_H_
#define PORTDEVICE_H_

#include <stdint.h>

class PortDevice {
public:
	PortDevice();
	virtual ~PortDevice();

	virtual void pread(uint16_t port, uint8_t *value) = 0;
	virtual void pwrite(uint16_t port, uint8_t value) = 0;
};

#endif /* PORTDEVICE_H_ */
