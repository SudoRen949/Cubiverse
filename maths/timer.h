#pragma once

#ifndef TIMER_H_
#define TIMER_H_

/*
 * [ Simple Timer ]
 *
 * Although SFML provides some timer function
 * called "Clock", I just wanted to be practical and simple.
 *
 */


#include "../defines.h"

class Timer
{
	private:

		UInt timer;
		UInt boundTime;

	public:

		void wait(UInt until);
		void reset(bool checkBound = true);
		bool reached();
};

#endif // TIMER_H_
