#pragma once

#ifndef _VOX2_MATH_H_
#define _VOX2_MATH_H_

#include <ctime>
#include <cmath>
#include <random>
#include <vector>
#include <iostream>

#include "../defines.h"

#define PI 3.141518927

class Math
{
	public:

		static void initRandom();

		static float degtorad(float val);
		static float sqr(float val);
		static float min(float a, float b);
		static float max(float a, float b);
		static float clamp(float val, float a, float b);
		static float random(float x);
		static float randomRange(float a, float b);
		static float choose(std::vector<float> sets);
		static float distance(std::vector<float> pointA, std::vector<float> pointB);
		static float floor(float v);

};

#endif /* _VOX2_MATH_H_ */
