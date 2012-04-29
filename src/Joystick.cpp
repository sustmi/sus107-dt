/*
 * Joystick.cpp
 *
 *  Created on: 25.4.2012
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#include "Joystick.h"

Joystick::Joystick() {
	pullDownData = 0xff; // no key pressed
}

Joystick::~Joystick() {
}

void Joystick::keyPressed(JoystickKeys key)
{
	if (key >= 0 && key < JOY_LAST) {
		pullDownData &= ~(1 << key); // pressed key pulls line down
	}
}

void Joystick::keyReleased(JoystickKeys key)
{
	if (key >= 0 && key < JOY_LAST) {
		pullDownData |= (1 << key); // released key releases line
	}
}

void Joystick::pread(uint16_t port, uint8_t *value)
{
	if (port == 0xf7fe) {
		*value = *value & pullDownData;
	}
}

void Joystick::pwrite(uint16_t port, uint8_t value)
{
	// joystick is read-only
}





