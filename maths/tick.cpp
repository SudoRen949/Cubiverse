
#include "tick.h"


Tick::Tick()
{
	tickTimer = 0;
	tickSpeed = DEFAULT_TICK_SPEED;
}

bool Tick::wait()
{
	if (tickTimer <= tickSpeed)
	{
		tickTimer++;
		return true;
	}
	return false;
}

void Tick::reset()
{
	if (tickTimer >= tickSpeed+1)
		tickTimer = 0;
}

void Tick::setTickSpeed(UInt newSpeed)
{
	if (newSpeed <= 0)
		newSpeed = DEFAULT_TICK_SPEED;
	tickSpeed = newSpeed;
}
