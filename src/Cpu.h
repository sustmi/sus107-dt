// Cpu.h

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

#ifndef CPU_H_
#define CPU_H_

#include <z80ex.h>
#include "Ula.h"
#include "Ports.h"

class Machine;
class Ula;

class Cpu {
public:
	Cpu();
	virtual ~Cpu();

	void attach(Ula *ula, Ports *ports);

	void reset();

	void setRegister(Z80_REG_T reg, Z80EX_WORD value);
	Z80EX_WORD getRegister(Z80_REG_T reg);

	uint64_t step();
	uint64_t stepInstruction();
	void setIntLineState(bool state);
	void setNmiLineState(bool state);

	uint64_t getOpTime();
	void wait(int tstates);

private:
	Z80EX_CONTEXT *cpu_context;

	Ula *ula;
	Ports *ports;

	bool intRequest;
	bool intAck;

	bool nmiState;
	bool nmiStateSampled;
	bool intState;
	bool intStateSampled;

	Z80EX_BYTE mread(Z80EX_WORD addr, int m1_state);
	void mwrite(Z80EX_WORD addr, Z80EX_BYTE value);
	Z80EX_BYTE pread(Z80EX_WORD port);
	void pwrite(Z80EX_WORD port, Z80EX_BYTE value);
	Z80EX_BYTE intread();

	// wrapper friend functions
	friend Z80EX_BYTE cb_mread(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, int m1_state, void *user_data);
	friend void cb_mwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, Z80EX_BYTE value, void *user_data);
	friend Z80EX_BYTE cb_pread(Z80EX_CONTEXT *cpu, Z80EX_WORD port, void *user_data);
	friend void cb_pwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD port, Z80EX_BYTE value, void *user_data);
	friend Z80EX_BYTE cb_intread(Z80EX_CONTEXT *cpu, void *user_data);

};

Z80EX_BYTE cb_mread(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, int m1_state, void *user_data);
void cb_mwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, Z80EX_BYTE value, void *user_data);
Z80EX_BYTE cb_pread(Z80EX_CONTEXT *cpu, Z80EX_WORD port, void *user_data);
void cb_pwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD port, Z80EX_BYTE value, void *user_data);
Z80EX_BYTE cb_intread(Z80EX_CONTEXT *cpu, void *user_data);

#endif /* CPU_H_ */
