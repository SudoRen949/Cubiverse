
/* A Code from Github... */

#include "noise.h"

Noise::Noise(int seed)
{
	permutation.resize(256);
	std::iota(permutation.begin(),permutation.end(),0);
	std::default_random_engine engine(seed/256);
	std::shuffle(permutation.begin(),permutation.end(),engine);
	permutation.insert(permutation.end(),permutation.begin(),permutation.begin());
}

float Noise::getHeight(float x, float y)
{
	float total = 0.0f, p = 0.69f, oct = 1.0f, freq, amp;
	for (int i = 0; i < oct; ++i)
	{
		freq = pow(2,i);
		amp = pow(p,i);
		total += interNoise(x * freq / (CHUNK_SIZE+8), y * freq / (CHUNK_SIZE+8)) * amp;
	}
	return total;
}

/* -- PRIVATE FUNCTIONS */

float Noise::interp(float a, float b, float x)
{
	float ft = x * PI;
	float f  = (1-cos(ft)) * 0.5;
	return a * (1-f) + b * f;
}

float Noise::noise(int x, int y)
{
	int n = x + y * 128; // 57
	n = (n << 13) ^ n;
	return (1.0 - ((n * (n * n * permutation[0] + permutation[1]) + permutation[2]) & 0x7FFFFFFF) / 1073741824.0);
}

float Noise::smooth(float x, float y)
{
	float corners 	= (noise(x-1,y-1)+noise(x+1,y-1)+noise(x-1,y+1)+noise(x+1,y+1)) / 16;
	float sides 	= (noise(x-1,y)+noise(x+1,y)+noise(x,y-1)+noise(x,y+1)) / 8;
	float center 	= noise(x,y) / 4;
	return corners + sides + center;
}

float Noise::interNoise(float x, float y)
{
	int X = static_cast<int>(x), Y = static_cast<int>(y);
	float fracX = x - X;
	float fracY = y - Y;
	float v1 = smooth(X  ,  Y);
	float v2 = smooth(X+1,	 Y);
	float v3 = smooth(X  ,Y+1);
	float v4 = smooth(X+1,Y+1);
	float i1 = interp(v1,v2,fracX);
	float i2 = interp(v3,v4,fracX);
	return interp(i1,i2,fracY);
}
