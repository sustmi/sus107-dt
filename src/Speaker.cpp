/*
 * Speaker.cpp
 *
 *  Created on: 19.3.2012
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#include <cmath>

#include "Speaker.h"

Speaker::Speaker() {
	PaError err;

	err = Pa_Initialize();
	if (err != paNoError) {
		printf("paError\n");
		exit(1); // TODO: error handling
	}

	PaStreamParameters outputParameters;
	outputParameters.device = Pa_GetDefaultOutputDevice(); // default output device
	outputParameters.channelCount = 1; // mono
	outputParameters.sampleFormat = paFloat32; // float <-1.0, 1.0>
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	err = Pa_OpenStream(
			  &stream,
			  NULL, // no input parameters
			  &outputParameters,
			  22050, // sampling rate
			  paFramesPerBufferUnspecified, // variable buffer length
			  paClipOff, // no output clipping
			  ::paCallback,
			  this );

	if (err != paNoError) {
		printf("paError2\n");
		exit(1); // TODO: error handling
	}

	pthread_mutex_init(&mutex, NULL);
}

Speaker::~Speaker() {
	Pa_Terminate();
	pthread_mutex_destroy(&mutex);
}

void Speaker::attach(Machine *machine)
{
	this->machine = machine;
}

void Speaker::start()
{
	// start with 0.5s lag, so that soundBuffer always contains enough data for paCallback
	lastFrameTime = machine->getCurrentTime() - (uint64_t)(0.5 * machine->getCpuFreq());
	Pa_StartStream(stream);
}

void Speaker::stop()
{
	Pa_StopStream(stream);
}

void Speaker::setSignal(double level) {
	uint64_t time = machine->getCurrentTime();

	pthread_mutex_lock(&mutex);

	if (soundBuffer.empty()) {
		SpeakerSample soundEvent;
		soundEvent.time = time;
		soundEvent.value = level;
		soundBuffer.push_back(soundEvent);
	} else {
		SpeakerSample lastSoundEvent;
		lastSoundEvent = soundBuffer.back();
		if (lastSoundEvent.time < time && lastSoundEvent.value != level) {
			SpeakerSample soundEvent;
			soundEvent.time = time;
			soundEvent.value = level;
			soundBuffer.push_back(soundEvent);
		}
	}

	pthread_mutex_lock(&mutex);
}

int Speaker::paCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData)
{
	uint64_t bufferLenTstates = ((framesPerBuffer) / 22050.0) * machine->getCpuFreq();

	uint64_t startTstate = this->lastFrameTime;
	uint64_t endTstate = startTstate + bufferLenTstates;

	SpeakerSample event;

	pthread_mutex_lock(&mutex);

	while (!soundBuffer.empty()) {
		event = soundBuffer.front();
		soundBuffer.pop_front();

		if (event.time < startTstate) {
			((float *)outputBuffer)[0] = event.value;
		} else {
			break;
		}
	}

	for (int i = 0; i < framesPerBuffer; i++) {
		uint64_t tstate = startTstate + i * (((double)(endTstate - startTstate)) / framesPerBuffer);

		while (tstate >= event.time && !soundBuffer.empty()) {
			event = soundBuffer.front();
			soundBuffer.pop_front();
		}

		((float *)outputBuffer)[i] = event.value;
	}

	// vrátit zpět ten, co patří až do dalšího okna
	soundBuffer.push_front(event);

	pthread_mutex_unlock(&mutex);

	lastFrameTime += bufferLenTstates;

	return paContinue;
}

int paCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData)
{
	Speaker *speaker = (Speaker *)userData;
	return speaker->paCallback(inputBuffer, outputBuffer,
			framesPerBuffer,
			timeInfo,
			statusFlags,
			userData);
}


