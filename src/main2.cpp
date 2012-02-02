#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include "Machine.h"
#include "Cpu.h"
#include "Ula.h"
#include "Memory.h"
#include "Ports.h"

int main (int argc, char *argv[])
{
	Memory *memory = new Memory();
	Cpu *cpu = new Cpu();
	Ula *ula = new Ula();
	Ports *ports = new Ports();
	TapeRecorder *tapeRecorder = new TapeRecorder();

	Machine *machine = new Machine();
	machine->attach(memory, cpu, ula);

	memory->attach(machine);
	cpu->attach(ula, ports);
	ula->attach(machine, memory, cpu, tapeRecorder);
	tapeRecorder->attach(machine);

	ports->connectDevice(ula);

	machine->loadSnapshot("manicminer.z80");
	machine->loadRom("48.rom");

	//tapeRecorder->load("manicminer.tap");
	//tapeRecorder->play();

	/*for (int i = 0; i < 128; i++) {
		printf("0x%04x: 0x%02x\n", i, machine->dasm_readbyte(i));
	}*/

	char buf[1024];

	/*printf("zacatek: t = %d\n", machine->getCurrentTime());
	for (int i = 0; i < 100; i++) {
		uint16_t addr = machine->getPC();
		int dummy;
		machine->dasm(buf, 1024, 0, &dummy, &dummy, addr);
		printf("PC = 0x%04x: %s\n", machine->getPC(), buf);
		machine->stepInstruction();
	}
	printf("konec: t = %d\n", machine->getCurrentTime());*/


	cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);
	IplImage *img = cvCreateImage(cvSize(352, 288), IPL_DEPTH_8U, 4);
	uint32_t buffer[288][352];



	while (1) {
		for (int i = 0; i < 500000; i++) {
			machine->stepInstruction();
		}

		ula->renderScreen(&buffer);
		for (int y = 0; y < 288; y++) {
			for (int x = 0; x < 352; x++) {
				CV_IMAGE_ELEM(img, uint32_t, y, x) = buffer[y][x];
			}
		}

		cvShowImage("Image", img);
		cvWaitKey( 0 );
	}

	delete memory;
	delete machine;
	delete ula;
	delete cpu;


	return 0;
}
