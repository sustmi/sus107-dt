/*
 * Beeper.cpp
 *
 *  Created on: 19.3.2012
 *      Author: mirek
 */

#include <cmath>

#include "Beeper.h"

Beeper::Beeper() {
	PaError err;

	err = Pa_Initialize();
	if (err != paNoError) {
		printf("paError\n");
		exit(1);
	}

	err = Pa_OpenDefaultStream( &stream,
		0,          /* no input channels */
		1,          /* mono output */
		paFloat32,  /* 32 bit floating point output */
		22050,
		256,        /* frames per buffer, i.e. the number
						   of sample frames that PortAudio will
						   request from the callback. Many apps
						   may want to use
						   paFramesPerBufferUnspecified, which
						   tells PortAudio to pick the best,
						   possibly changing, buffer size.*/
		paCallback, /* this is your callback function */
		this ); /*This is a pointer that will be passed to
						   your callback*/

	if (err != paNoError) {
		printf("paError2\n");
		exit(1);
	}
}

Beeper::~Beeper() {
	// TODO Auto-generated destructor stub
	Pa_Terminate();
}

void Beeper::attach(Machine *machine, Ula *ula)
{
	this->machine = machine;
	this->ula = ula;
}

int paCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData)
{
	Beeper *beeper = (Beeper *)userData;

	uint64_t bufferTstatesLen = (framesPerBuffer / 22050.0) * beeper->machine->getCpuFreq();

	uint64_t endTstate = beeper->machine->getCurrentTime();
	//uint64_t endTstate = (((double)(beeper->machine->getCurrentTime() - beeper->startTStates)) / (timeInfo->currentTime - beeper->streamTime)) * (timeInfo->outputBufferDacTime - timeInfo->currentTime) + beeper->startTStates;
	uint64_t startTstate = endTstate - bufferTstatesLen;

	/*printf("startTstate = %ld, endTstate = %ld\n", startTstate, endTstate);

	if (statusFlags & paInputUnderflow) {
		printf("paInputUnderflow ");
	}
	if (statusFlags & paInputOverflow) {
		printf("paInputOverflow ");
	}
	if (statusFlags & paOutputUnderflow) {
		printf("paOutputUnderflow ");
	}
	if (statusFlags & paOutputOverflow) {
		printf("paOutputOverflow ");
	}
	if (statusFlags & paPrimingOutput) {
		printf("paPrimingOutput ");
	}
	printf("\n");*/

	SoundEvent event;

	if (!beeper->ula->soundBuffer.empty()) {
		//printf("startBuffer = %ld, endBuffer = %ld\n", beeper->ula->soundBuffer.front().time, beeper->ula->soundBuffer.back().time);
	}

	int m = 0;

	while (!beeper->ula->soundBuffer.empty()) {
		event = beeper->ula->soundBuffer.front();
		beeper->ula->soundBuffer.pop_front();

		m++;

		if (event.time < startTstate) {
			((float *)outputBuffer)[0] = event.value;
		} else {
			break;
		}
	}

	int n = 0;

	for (int i = 0; i < framesPerBuffer; i++) {
		uint64_t tstate = startTstate + i * (((double)(endTstate - startTstate)) / framesPerBuffer);
		//printf("tstate = %ld, event.time = %ld\n", tstate, event.time);

		while (tstate >= event.time && !beeper->ula->soundBuffer.empty()) {
			event = beeper->ula->soundBuffer.front();
			beeper->ula->soundBuffer.pop_front();
			n++;
		}

		((float *)outputBuffer)[i] = event.value;
	}

	//printf("buffer %d/%d/%d\n", m, n, framesPerBuffer);

	return 0;
}

void Beeper::Start()
{
	Pa_StartStream(stream);

	startTStates = machine->getCurrentTime();
	streamTime = Pa_GetStreamTime(stream);
}

void Beeper::Stop()
{
	Pa_StopStream(stream);
}


