/*
 * Machine.cpp
 *
 *  Created on: 8.12.2011
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#include <stdio.h>

#include <z80ex.h>
#include <z80ex_dasm.h>
#include <libspectrum.h>

#include <queue>

#include "Machine.h"

Machine::Machine() {
	currentTime = 0;
	inCallback = false;

	libspectrum_init();
}

Machine::~Machine() {
}

void Machine::attach(Memory *memory, Cpu *cpu, Ula *ula) {
	this->memory = memory;
	this->cpu = cpu;
	this->ula = ula;
}
Cpu *Machine::getCpu()
{
	return cpu;
}
Memory *Machine::getMemory()
{
	return memory;
}
Ula *Machine::getUla()
{
	return ula;
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

	inCallback = true;
	currentTime += cpu->step();
	inCallback = false;
}

void Machine::stepInstruction() {
	cpu->setIntLineState(ula->getIntLineState());

	inCallback = true;
	currentTime += cpu->stepInstruction();
	inCallback = false;
}

uint64_t Machine::getCurrentTime()
{
	if (inCallback) {
		return currentTime + cpu->getOpTime();
	} else {
		return currentTime;
	}
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














