/*
 * Machine.cpp
 *
 *  Created on: 8.12.2011
 *      Author: mirek
 */

#include <stdio.h>

#include <z80ex.h>
#include <z80ex_dasm.h>
#include <libspectrum.h>

#include <queue>

#include "Machine.h"

Machine::Machine() {
	currentTime = 0;

	libspectrum_init();
}

Machine::~Machine() {
}

void Machine::attach(Memory *memory, Cpu *cpu, Ula *ula) {
	this->memory = memory;
	this->cpu = cpu;
	this->ula = ula;
}

bool Machine::loadRom(const char *filename) {
	FILE *f = fopen(filename, "rb");
	fseek(f, 0 , SEEK_END);
	long fsize = ftell(f);
	rewind(f);

	uint8_t *buffer = new uint8_t[fsize];
	fread(buffer, sizeof(char), fsize, f);
	fclose(f);

	memory->writePage(0, buffer, fsize);

	delete buffer;

	return true;
}

bool Machine::loadSnapshot(const char *filename) {
	libspectrum_snap *snap = libspectrum_snap_alloc();

	FILE *f = fopen(filename, "rb");
	fseek(f, 0 , SEEK_END);
	long fsize = ftell(f);
	rewind(f);

	uint8_t *buffer = new uint8_t[fsize];
	fread(buffer, sizeof(uint8_t), fsize, f);
	fclose(f);

	libspectrum_error error = libspectrum_snap_read(snap, (libspectrum_byte*)buffer, fsize,
			LIBSPECTRUM_ID_UNKNOWN, filename);

	cpu->reset();

	// load CPU registers
	cpu->setRegister(regAF, (libspectrum_snap_a(snap) << 8) | (libspectrum_snap_f(snap)));
	cpu->setRegister(regAF, (libspectrum_snap_a(snap) << 8) | (libspectrum_snap_f(snap)));;
	cpu->setRegister(regBC, libspectrum_snap_bc(snap));
	cpu->setRegister(regDE, libspectrum_snap_de(snap));
	cpu->setRegister(regHL, libspectrum_snap_hl(snap));
	cpu->setRegister(regAF_, (libspectrum_snap_a_(snap) << 8) | (libspectrum_snap_f_(snap)));
	cpu->setRegister(regBC_, libspectrum_snap_bc_(snap));
	cpu->setRegister(regDE_, libspectrum_snap_de_(snap));
	cpu->setRegister(regHL_, libspectrum_snap_hl_(snap));
	cpu->setRegister(regIX, libspectrum_snap_ix(snap));
	cpu->setRegister(regIY, libspectrum_snap_iy(snap));
	cpu->setRegister(regPC, libspectrum_snap_pc(snap));
	cpu->setRegister(regSP, libspectrum_snap_sp(snap));
	cpu->setRegister(regI, libspectrum_snap_i(snap));
	cpu->setRegister(regR, libspectrum_snap_r(snap));
	//cpu->setRegister(regR7, TODO);
	cpu->setRegister(regIM, libspectrum_snap_im(snap));
	cpu->setRegister(regIFF1, libspectrum_snap_iff1(snap));
	cpu->setRegister(regIFF2, libspectrum_snap_iff2(snap));

	// load ROM
	const libspectrum_byte *rom;
	int rom_size;
	rom = libspectrum_snap_roms(snap, 0);
	rom_size = libspectrum_snap_rom_length(snap, 0);
	memory->writePage(0, rom, rom_size);

	// load memory pages
	const libspectrum_byte *page;
	// 0x4000-0x7fff = pages[5]
	page = libspectrum_snap_pages(snap, 5);
	memory->writePage(1, page, 0x4000);
	// 0x8000-0xbfff = pages[2]
	page = libspectrum_snap_pages(snap, 2);
	memory->writePage(2, page, 0x4000);
	// 0xc000-0xffff = pages[0]
	page = libspectrum_snap_pages(snap, 0);
	memory->writePage(3, page, 0x4000);

	// set current emulation time
	currentTime = libspectrum_snap_tstates(snap);

	// Spectrum 48k machine specifics
	ula->setInterruptLength(libspectrum_timings_interrupt_length(LIBSPECTRUM_MACHINE_48));
	ula->setInterruptPeriod(libspectrum_timings_tstates_per_frame(LIBSPECTRUM_MACHINE_48));
	setCpuFreq(libspectrum_timings_processor_speed(LIBSPECTRUM_MACHINE_48));

	libspectrum_snap_free(snap);
	delete buffer;

	return true;
}

void Machine::step() {
	// for each Event where Event->time <= current time
	//   Event->invoke()

	// z80_step()

	/*int event;
	while ((event = eventQueue.top()) <= currentTime) {
		eventQueue.pop();
		currentTime = event;
		//Event->invoke();
		// - může v CPU nastavit interruptRequest
	}*/

	cpu->setIntLineState(ula->getIntLineState());

	currentTime += cpu->step();
}

void Machine::stepInstruction() {
	cpu->setIntLineState(ula->getIntLineState());

	currentTime += cpu->stepInstruction();
}

void Machine::setCpuFreq(uint64_t cpuFreq) {
    this->cpuFreq = cpuFreq;
}
uint64_t Machine::getCpuFreq() const {
    return cpuFreq;
}

Z80EX_WORD Machine::getPC() {
	return cpu->getRegister(regPC);
}

int Machine::dasm(char *output, int output_size, unsigned  flags, int *t_states, int *t_states2, Z80EX_WORD addr) {
	return z80ex_dasm(output, output_size, flags, t_states, t_states2, ::dasm_readbyte, addr, this);
}

void Machine::start() {
	if (!running) {

		running = true;
	}
}

void Machine::stop() {
	if (running) {

	}
}

void Machine::onTick() {
}

Z80EX_BYTE Machine::dasm_readbyte(Z80EX_WORD addr) {
	return this->memory->rawRead(addr);
}


Z80EX_BYTE dasm_readbyte(Z80EX_WORD addr, void *user_data) {
	Machine *machine = (Machine *)user_data;
	return machine->dasm_readbyte(addr);
}














