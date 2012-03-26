/*
 * Beeper.h
 *
 *  Created on: 19.3.2012
 *      Author: mirek
 */

#ifndef BEEPER_H_
#define BEEPER_H_

#include <portaudio.h>

#include "Machine.h"
#include "Ula.h"

class Beeper {
public:
	Beeper();
	virtual ~Beeper();

	void attach(Machine *machine, Ula *ula);

	void Start();
	void Stop();

	Machine *machine;
	Ula *ula;

	uint64_t startTStates;
	PaTime streamTime;

private:
	/*Machine *machine;
	Ula *ula;*/

	PaStream *stream;

	/*uint64_t startTStates;
	PaTime streamTime;*/
};

int paCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData);

#endif /* BEEPER_H_ */
