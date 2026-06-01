
#include "maths.h"

void Math::initRandom()
{
	srand(static_cast<unsigned>(time(NULL)));
}

float Math::degtorad(float val)
{
	return (val) / 180 * PI;
}

float Math::sqr(float val)
{
	return (val) * (val);
}

float Math::min(float a, float b)
{
	return (a < b) ? a : b;
}

float Math::max(float a, float b)
{
	return (a > b) ? a : b;
}

float Math::clamp(float val, float a, float b)
{
	return min(max(val,a),b);
}

float Math::random(float x)
{
	return (static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / x));
}

float Math::randomRange(float a, float b)
{
	return (static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (a - b))) + b;
}

float Math::choose(std::vector<float> sets)
{
	UInt sizeOfSets = static_cast<UInt>(sets.size());
	UInt randomNumber = static_cast<UInt>(Math::randomRange(1,sizeOfSets));

	return sets[randomNumber-1];
}

float Math::distance(std::vector<float> pointA,
std::vector<float> pointB)
{
	if (pointA.size() > 3 || pointB.size() > 3 ||
		pointA.size() <= 0 || pointB.size() <= 0)
	{
		std::cout << "Cannot calculate distance with a 4 dimensional or 0 dimensional vector";
		return -3.0f;
	}

	if 		(pointA.size() == 1 && pointB.size() == 1) return sqrt( sqr(pointA[0] - pointB[0]) );
	else if (pointA.size() == 2 && pointB.size() == 2) return sqrt( sqr(pointA[0] - pointB[0]) + sqr(pointA[1] - pointB[1]) );
	else if (pointA.size() == 3 && pointB.size() == 3) return sqrt( sqr(pointA[0] - pointB[0]) + sqr(pointA[1] - pointB[1]) + sqr(pointA[2] - pointB[2]) );
	else
	{
		std::cout << "Two points must be in the same dimension/size.";
		return 0.0f;
	}

	return 0.0f;
}

float Math::floor(float v)
{
	int i = static_cast<int>(i);
	return (v < i) ? (i-1) : i ;
}


