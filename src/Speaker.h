// Speaker.h

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

#ifndef SPEAKER_H_
#define SPEAKER_H_

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

#endif /* SPEAKER_H_ */
