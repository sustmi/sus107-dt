/*
 * Emulator.h
 *
 *  Created on: 20.3.2012
 *      Author: mirek
 */

#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "Machine.h"
#include "Cpu.h"
#include "Ula.h"
#include "TapeRecorder.h"
#include "Keyboard.h"

#include "util/Timer.h"
#include "Debugger.h"

#include "Beeper.h"

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
	TapeRecorder *getTapeRecorder() const;
	void renderScreen(uint32_t *buffer);

private:
	bool running;

	Timer *timer;
	Debugger *debugger;

	Machine *machine;
	Cpu *cpu;
	Ula *ula;
	TapeRecorder *tapeRecorder;
	Keyboard *keyboard;

	//Beeper *beeper;
};

#endif /* EMULATOR_H_ */
