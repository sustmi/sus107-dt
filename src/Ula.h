// Ula.h

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

#ifndef ULA_H_
#define ULA_H_

#include <deque>

#include "Machine.h"
#include "Memory.h"
#include "Cpu.h"
#include "PortDevice.h"
#include "Keyboard.h"
#include "TapeRecorder.h"
#include "Speaker.h"

// forward declaration
class Machine;
class Memory;
class Cpu;
class Keyboard;
class TapeRecorder;
class Speaker;

class Ula : public PortDevice {
public:
	Ula();
	virtual ~Ula();

	void attach(Machine *machine, Memory *memory, Cpu *cpu, Keyboard *keyboard,
			TapeRecorder *tapeRecorder, Speaker *speaker);

	uint8_t mread(uint16_t addr);
	void mwrite(uint16_t addr, uint8_t value);

	bool getIntLineState();
	void setInterruptLength(uint64_t interruptLength);
	void setInterruptPeriod(uint64_t interruptPeriod);
	void setLastInterruptTime(uint64_t lastInterruptTime);

	// PortDevice methods
	void pread(uint16_t port, uint8_t *value);
	void pwrite(uint16_t port, uint8_t value);

	void renderScreen(uint32_t *buffer);

private:
	Machine *machine;
	Memory *memory;
	Cpu *cpu;

	Keyboard *keyboard;

	TapeRecorder *tapeRecorder;
	Speaker *speaker;

	uint64_t lastInterruptTime;
	uint64_t interruptPeriod;
	uint64_t interruptLength;

	int borderColor;

	uint64_t getTimeSinceLastInterrupt();

	void contendedMemoryDelay(uint16_t addr);
	void contendedPortDelay(uint16_t port);
	int contendedDelay(uint64_t tstates);
};

#endif /* ULA_H_ */
