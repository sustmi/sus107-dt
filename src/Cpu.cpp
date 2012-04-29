// Cpu.cpp

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

#include "Cpu.h"

#include <z80ex.h>
#include "Memory.h"
#include "Ports.h"

Cpu::Cpu() {
	intState = false;
	intStateSampled = false;
	nmiState = false;
	nmiStateSampled = false;

	cpu_context = z80ex_create(::cb_mread, (void *)this,
			::cb_mwrite, (void *)this,
			::cb_pread, (void *)this,
			::cb_pwrite, (void *)this,
			::cb_intread, (void *)this);
}

Cpu::~Cpu() {
	z80ex_destroy(cpu_context);
}

void Cpu::attach(Ula *ula, Ports *ports) {
	this->ula = ula;
	this->ports = ports;
}

void Cpu::reset() {
	z80ex_reset(cpu_context);
}

void Cpu::setRegister(Z80_REG_T reg, Z80EX_WORD value) {
	z80ex_set_reg(cpu_context, reg, value);
}
Z80EX_WORD Cpu::getRegister(Z80_REG_T reg)
{
	return z80ex_get_reg(cpu_context, reg);
}

uint64_t Cpu::step()
{
	int tstates = 0;

	if (z80ex_last_op_type(cpu_context) == 0) {
		if (nmiState && !nmiStateSampled && z80ex_nmi_possible(cpu_context)) {
			tstates = z80ex_nmi(cpu_context);
		} else if (intState && !intStateSampled && z80ex_int_possible(cpu_context)) {
			tstates = z80ex_int(cpu_context);
		}

		intStateSampled = intState;
		nmiStateSampled = nmiState;
	}

	if (tstates == 0) { // neither INT nor NMI acknowledged
		tstates = z80ex_step(cpu_context);
	}

	return tstates;
}

// TODO: skips over interrupt
uint64_t Cpu::stepInstruction()
{
	int tstates = 0;

	do {
		tstates += this->step();
	} while (z80ex_last_op_type(cpu_context) != 0);

	return tstates;
}

void Cpu::setIntLineState(bool state) {
	intState = state;
}
void Cpu::setNmiLineState(bool state) {
	nmiState = state;
}

Z80EX_BYTE Cpu::mread(Z80EX_WORD addr, int m1_state) {
	return ula->mread(addr);
}
void Cpu::mwrite(Z80EX_WORD addr, Z80EX_BYTE value) {
	ula->mwrite(addr, value);
}
Z80EX_BYTE Cpu::pread(Z80EX_WORD port) {
	return ports->pread(port);
}
void Cpu::pwrite(Z80EX_WORD port, Z80EX_BYTE value) {
	ports->pwrite(port, value);
}
Z80EX_BYTE Cpu::intread() {
	return 0; // TODO implement
}

void Cpu::wait(int tstates) {
	z80ex_w_states(cpu_context, tstates);
}

uint64_t Cpu::getOpTime() {
	return z80ex_op_tstate(cpu_context);
}

// wrapper friend functions
Z80EX_BYTE cb_mread(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, int m1_state, void *user_data) {
	Cpu *_cpu = (Cpu *)user_data;
	return _cpu->mread(addr, m1_state);
}
void cb_mwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, Z80EX_BYTE value, void *user_data) {
	Cpu *_cpu = (Cpu *)user_data;
	_cpu->mwrite(addr, value);
}
Z80EX_BYTE cb_pread(Z80EX_CONTEXT *cpu, Z80EX_WORD port, void *user_data) {
	Cpu *_cpu = (Cpu *)user_data;
	return _cpu->pread(port);
}
void cb_pwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD port, Z80EX_BYTE value, void *user_data) {
	Cpu *_cpu = (Cpu *)user_data;
	_cpu->pwrite(port, value);
}

Z80EX_BYTE cb_intread(Z80EX_CONTEXT *cpu, void *user_data) {
	Cpu *_cpu = (Cpu *)user_data;
	return _cpu->intread();
}





