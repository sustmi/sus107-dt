/*
 * Timer.h
 *
 *  Created on: 10.1.2012
 *      Author: mirek
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <wx/timer.h>
#include <wx/stopwatch.h>

class Timer {
public:
	Timer();
	virtual ~Timer();

	void setTime(long miliseconds);
	void setPeriod(long miliseconds);
	void setCallback();
	void start();
	void stop();

private:
	long timeout;
	bool running;

	wxTimer *timer;
	wxStopWatch *stopWatch;
};

#endif /* TIMER_H_ */
