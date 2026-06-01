#pragma once

#ifndef TICK_H_
#define TICK_H_

#include "../defines.h"

class Tick
{
	private:

		UInt tickTimer, tickSpeed;

	public:

		Tick();

		bool wait();
		void reset();
		void setTickSpeed(UInt newSpeed);
};

#endif // TICK_H_
