// Machine.h

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

#ifndef MACHINE_H_
#define MACHINE_H_

#include "Cpu.h"
#include "Ula.h"
#include "Memory.h"
#include <z80ex_dasm.h>

#include <queue>

// forward declaration
class Cpu;
class Ula;
class Memory;

class Machine {
public:
	Machine();
	virtual ~Machine();

	void attach(Memory *memory, Cpu *cpu, Ula *ula);
	Cpu *getCpu();
	Memory *getMemory();
	Ula *getUla();

	void step();
	void stepInstruction();
	bool loadRom(const char *filename);
	bool loadSnapshot(const char *filename);

	uint64_t getCurrentTime();
	//void addEvent(int event);

	void setCpuFreq(uint64_t cpuFreq);
	uint64_t getCpuFreq() const;

private:

	Cpu *cpu;
	Ula *ula;
	Memory *memory;

	// current emulation time (TODO: in t-states?)
	uint64_t currentTime;
	uint64_t cpuFreq;

	bool interruptRequest;
	bool inCallback;

	std::priority_queue<int> eventQueue;

	bool running;
};


#endif /* MACHINE_H_ */
