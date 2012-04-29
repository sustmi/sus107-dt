/*
 * Speaker.h
 *
 *  Created on: 19.3.2012
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#ifndef BEEPER_H_
#define BEEPER_H_

#include <portaudio.h>
#include <pthread.h>

#include "Machine.h"

class Machine;

struct SpeakerSample {
	uint64_t time;
	float value;
};

class Speaker {
public:
	Speaker();
	virtual ~Speaker();

	void attach(Machine *machine);

	void start();
	void stop();
	void setSignal(double level);

	int paCallback(const void *inputBuffer, void *outputBuffer,
			unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags,
			void *userData);

private:
	Machine *machine;

	PaStream *stream;

	uint64_t lastFrameTime;
	std::deque<SpeakerSample> soundBuffer;
	pthread_mutex_t mutex;
};

int paCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);

#endif /* BEEPER_H_ */
