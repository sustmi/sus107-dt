/*
 * Clock.cpp
 *
 *  Created on: 30.4.2012
 *      Author: mirek
 */

#include "Clock.h"

Clock::Clock() {
	timer = new wxTimer(this, wxID_ANY);
	stopWatch = new wxStopWatch;
	stopWatch->Pause();
	Connect(timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(Clock::OnTimer));
}

Clock::~Clock() {
	timer->Stop();
	stopWatch->Pause();
	Disconnect(timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(Clock::OnTimer));

	delete timer;
	delete stopWatch;
}

void Clock::attach(Emulator *emulator)
{
	this->emulator = emulator;
}

void Clock::start(uint64_t period_ms, uint64_t time_ms)
{
	timer->Start(period_ms);
	stopWatch->Start(time_ms);
}

void Clock::resume()
{
	timer->Start();
	stopWatch->Resume();
}

void Clock::stop()
{
	timer->Stop();
	stopWatch->Pause();
}

uint64_t Clock::getTime()
{
	return stopWatch->Time();
}

void Clock::OnTimer(wxTimerEvent &event)
{
	emulator->runUntil(stopWatch->Time());
}



