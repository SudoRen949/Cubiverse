#pragma once

#ifndef VOX2_SMPLX_H
#define VOX2_SMPLX_H

#include "maths.h"

class Simplex
{
	public:

		Simplex(float frequency = 1.0f, float amplitude = 1.0f, float lacunarity = 2.0f, float persistence = 0.5f) :
			mFrequency(frequency),
			mAmplitude(amplitude),
			mLacunarity(lacunarity),
			mPersistence(persistence),
		{}

		static float noise(float x, float y);
		
		float fractal(size_t octaves, float x, float y) const;

	private:

		float mFrequency;
		float mAmplitude;
		float mLacunarity;
		float mPersistence;
};

#endif /* VOX2_SMPLX_H */