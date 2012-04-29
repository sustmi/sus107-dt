/*
 * Machine.h
 *
 *  Created on: 8.12.2011
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#ifndef MACHINE_H_
#define MACHINE_H_

#include "Cpu.h"
#include "Ula.h"
#include "Memory.h"
#include <z80ex_dasm.h>
#include "util/Timer.h"

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
	Z80EX_WORD getPC();

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
	Timer timer;
};


#endif /* MACHINE_H_ */
