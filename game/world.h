#pragma once

#ifndef _VOX2_WORLD_H_
#define _VOX2_WORLD_H_

#include "terrain.h"
#include "../maths/noise.h"
#include "../maths/raycast.h"
#include "../defines.h"
#include "skybox.h"
#include "clouds.h"
#include "../maths/tick.h"

class World
{
	public:

		World(int seed = 0);
		~World();

		void render(sf::Vector3f position, Raycast raycast);
		void setRenderDistance(int distance);

		int getRenderDistance();

		Grid getMap();
		Chunk* getChunk();

		void setViewProperty(sf::Vector3f props);

		UInt getSeed();
		UInt getNoiseHeight();

	private:

		int XX,YY,ZZ;
		int renderDistance;
		int updateDistance;

		float X1,Y1,Z1,X2,Y2,Z2;
		float size, radius;
		float fov, near, far;

		UInt tickTimer, seed;

		Tick tick[6];
		Grid worldMap;
		Chunk* chunk;
		Skybox skybox;
		Clouds clouds;
		Terrain terrain;
		Noise* perlinNoise;

	protected:

		void blockPhysics(Grid grid, sf::Vector3i position);
};

#endif /* _VOX2_WORLD_H_ */
