
#include "timer.h"

void Timer::wait(UInt until)
{
	if (until <= 0) return;

	boundTime = until;

	if (timer <= until) timer++;
}

void Timer::reset(bool checkBound)
{
	if (checkBound)
	{
		if (timer >= boundTime)
			timer = 0;
	}
	else timer = 0;
}

bool Timer::reached()
{
	if (timer >= boundTime) return true;
	return false;
}
