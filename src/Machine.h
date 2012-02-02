/*
 * Machine.h
 *
 *  Created on: 8.12.2011
 *      Author: mirek
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

	void step();
	void stepInstruction();
	bool loadRom(const char *filename);
	bool loadSnapshot(const char *filename);
	Z80EX_WORD getPC();

	uint64_t getCurrentTime() { return currentTime; }
	//void addEvent(int event);

	void start();
	void stop();
	void onTick();

	int dasm(char *output, int output_size, unsigned flags, int *t_states, int *t_states2, Z80EX_WORD addr);

	Z80EX_BYTE dasm_readbyte(Z80EX_WORD addr);

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

	std::priority_queue<int> eventQueue;

	bool running;
	Timer timer;
};

Z80EX_BYTE dasm_readbyte(Z80EX_WORD addr, void *user_data);


#endif /* MACHINE_H_ */
