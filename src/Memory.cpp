// Memory.cpp

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

#include <memory.h>

#include "Memory.h"

Memory::Memory() {
	memset(mem, 0, sizeof(mem));
}

Memory::~Memory() {
}

void Memory::attach(Machine *machine) {
	this->machine = machine;
}

uint8_t Memory::read(uint16_t addr) {
	return mem[addr];
}
void Memory::write(uint16_t addr, uint8_t value) {
	if (addr >= 0x4000) { // 0x0000-0x3fff is ROM
		mem[addr] = value;
	}
}

uint8_t Memory::rawRead(uint16_t addr) {
	return mem[addr];
}
void Memory::rawWrite(uint16_t addr, uint8_t value) {
	mem[addr] = value;
}

void Memory::writePage(int page, const uint8_t *data, int size) {
	memcpy(&mem[page*PAGE_SIZE], data, (size <= PAGE_SIZE) ? size : PAGE_SIZE);
}



