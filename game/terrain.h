#pragma once

#ifndef _VOX2_TERRAIN_H_
#define _VOX2_TERRAIN_H_

#include <iostream>
#include <SFML/System.hpp>

#include "../types.h"
#include "chunk.h"
#include "../maths/grid.h"
#include "../defines.h"
#include "../maths/noise.h"
#include "../maths/timer.h"

class Terrain
{
	private:

		void createLand(Chunk *chunk, Grid grid, int noiseHeight, int x, int z);
		void createCave(Chunk *chunk, Grid grid, int noiseHeight, int x, int z);
		void createSea(Chunk *chunk, Grid grid, int noiseHeight, int x, int z);
		void createTrees(Chunk *chunk, Grid grid, int noiseHeight, int x, int z);
		void createBushes(Chunk *chunk, Grid grid, int noiseHeight, int x, int z);

		UInt noiseHeight;

	public:

		void generate(Chunk *chunk, Grid grid, Noise *noise);

		UInt getNoiseHeight();
};

#endif /* _VOX2_TERRAIN_H_ */
