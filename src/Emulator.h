/*
 * Emulator.h
 *
 *  Created on: 20.3.2012
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "Machine.h"
#include "Cpu.h"
#include "Ula.h"
#include "TapeRecorder.h"
#include "Keyboard.h"
#include "Joystick.h"
#include "Speaker.h"

#include "util/Timer.h"
#include "Debugger.h"

class Debugger;

class Emulator {
public:
	Emulator();
	virtual ~Emulator();

	void init();
	void start();
	void stop();

	void runUntil(uint64_t time);
	bool isRunning();

	Debugger *getDebugger() const;
	Machine *getMachine() const;
	Keyboard *getKeyboard() const;
	Joystick *getJoystick() const;
	TapeRecorder *getTapeRecorder() const;
	Speaker *getSpeaker() const;
	void renderScreen(uint32_t *buffer);

private:
	bool running;

	Timer *timer;
	Debugger *debugger;

	Machine *machine;
	Cpu *cpu;
	Ula *ula;
	TapeRecorder *tapeRecorder;
	Speaker *speaker;
	Keyboard *keyboard;
	Joystick *joystick;
};

#endif /* EMULATOR_H_ */
