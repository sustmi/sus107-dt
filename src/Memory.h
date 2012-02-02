/*
 * Memory.h
 *
 *  Created on: 9.12.2011
 *      Author: mirek
 */

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

	uint8_t rawRead(uint16_t addr);
	void rawWrite(uint16_t addr, uint8_t value);

	void writePage(int page, const uint8_t *data, int size);

private:
	Machine *machine;

	static const int PAGE_SIZE = 0x4000;
	uint8_t mem[PAGE_SIZE*4];
};

#endif /* MEMORY_H_ */
