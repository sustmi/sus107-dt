/*
 * Joystick.h
 *
 *  Created on: 25.4.2012
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "PortDevice.h"

enum JoystickKeys {
	JOY_LEFT = 0, JOY_RIGHT, JOY_DOWN, JOY_UP, JOY_FIRE,
	JOY_LAST
};

class Joystick: public PortDevice {
public:
	Joystick();
	virtual ~Joystick();

	void keyPressed(JoystickKeys key);
	void keyReleased(JoystickKeys key);

	// PortDevice methods
	void pread(uint16_t port, uint8_t *value);
	void pwrite(uint16_t port, uint8_t value);

private:
	uint8_t pullDownData;
};

#endif /* JOYSTICK_H_ */
