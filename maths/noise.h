#pragma once

#ifndef _VOX2_NOISE_H_
#define _VOX2_NOISE_H_

/**
*	Disclaimer
*	I got this code somewhere tho :/
*	I don't remember the author of this.
*/

#include <ctime>
#include <cstdio>
#include <vector>
#include <random>
#include <cstdlib>
#include <algorithm>

#include "maths.h"

class Noise
{
	public:

		Noise(int seed);
		float getHeight(float x, float y);

	private:

		std::vector<int> permutation;
		float interp(float a, float b, float x);
		float noise(int x, int y);
		float smooth(float x, float y);
		float interNoise(float x, float y);
};

#endif /* _VOX2_NOISE_H_ */
