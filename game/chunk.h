#pragma once

#ifndef _VOX2_BLOCK_H_
#define _VOX2_BLOCK_H_

#include <iostream>
#include <SFML/OpenGL.hpp>

#include "../maths/grid.h"
#include "../maths/texture.h"
#include "../types.h"
#include "../defines.h"
#include "../maths/timer.h"

class Chunk
{
	public:

		Chunk(Grid grid);
		~Chunk();
		void addBlock(Grid grid, sf::Vector3i position, int block);
		void removeBlock(Grid grid, sf::Vector3i position);
		void drawUnBlendedChunk(Grid grid, sf::Vector3i position);
		void drawBlendedChunk(Grid grid, sf::Vector3i position);
		UInt getUpdateCount();
		void connectedTexture(bool enable);

	private:

		bool mergedTexure;
		int x1,y1,z1,x2,y2,z2;
		int updateCounter, updateCount;
		UShort top, bottom, left, right, back, front;
		Texture texClass;
		Grid buffer[2], update[2];
		Timer timer;

		void updateChunk(sf::Vector3i position);
		void updateNearChunk(sf::Vector3i position);

	protected:

		void initOpaqueChunk(Grid grid, sf::Vector3i position);
		void initTransparentChunk(Grid grid, sf::Vector3i position);
		void initBlendedChunk(Grid grid, sf::Vector3i position);
		void drawOpaqueFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block);
		void drawTransparentFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block);
		void drawBlendedFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block);
		void drawBushesFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block);
};

#endif /* _VOX2_CHUNK_H_ */
