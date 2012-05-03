// Memory.h

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

#ifndef MEMORY_H_
#define MEMORY_H_

#include "Machine.h"

//forward declaration
class Machine;

class Memory {
public:
	Memory();
	virtual ~Memory();

	void attach(Machine *machine);

	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t value);
	uint8_t rawRead(uint16_t addr);
	void rawWrite(uint16_t addr, uint8_t value);

	void writePage(int page, const uint8_t *data, int size);

private:
	Machine *machine;

	static const int SPECTRUM_PAGE_SIZE = 0x4000;
	uint8_t mem[SPECTRUM_PAGE_SIZE*4];
};

#endif /* MEMORY_H_ */
