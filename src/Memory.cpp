/*
 * Memory.cpp
 *
 *  Created on: 9.12.2011
 *      Author: mirek
 */

#include <memory.h>

#include "Memory.h"

Memory::Memory() {
	// TODO Auto-generated constructor stub
}

Memory::~Memory() {
	// TODO Auto-generated destructor stub
}

void Memory::attach(Machine *machine) {
	this->machine = machine;
}

uint8_t Memory::rawRead(uint16_t addr) {
	return mem[addr];
}
void Memory::rawWrite(uint16_t addr, uint8_t value) {
	if (addr >= 0x4000) { // 0x0000-0x3fff is ROM
		mem[addr] = value;
	}
}

void Memory::writePage(int page, const uint8_t *data, int size) {
	memcpy(&mem[page*PAGE_SIZE], data, (size <= PAGE_SIZE) ? size : PAGE_SIZE);
}



