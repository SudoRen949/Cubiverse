
#include "terrain.h"

void Terrain::createLand(Chunk *chunk, Grid grid, int noiseHeight, int x, int z)
{
	int dirtHeight = noiseHeight-Math::randomRange(2,7);
	int stoneHeight = Math::randomRange(3,7);

	for (int yy = 0; yy < noiseHeight; ++yy)
	{
		if (yy == noiseHeight-1) 						chunk->addBlock(grid,sf::Vector3i(x,yy,z),Types::GRASS);
		if (yy < noiseHeight-1 && yy >= dirtHeight) 	chunk->addBlock(grid,sf::Vector3i(x,yy,z),Types::DIRT);
		if (yy < dirtHeight && yy >= stoneHeight) 		chunk->addBlock(grid,sf::Vector3i(x,yy,z),Types::STONE);
		if (yy < stoneHeight && yy >= 0) 				chunk->addBlock(grid,sf::Vector3i(x,yy,z),Types::BEDROCK);
	}
}

void Terrain::createCave(Chunk *chunk, Grid grid, int noiseHeight, int x, int z)
{
	UInt seeds = Math::random(100);

	if (seeds == 5)
	{
		int rPY = Math::randomRange(15,noiseHeight-16);
		int X = Math::random(x), Z = Math::random(z);

		short wX = Math::randomRange(2,7), wZ = Math::randomRange(2,7);
		short deep = Math::randomRange(4,14);

		/*
		for (int xx = -wX; xx <= wX; xx++)
		for (int zz = -wZ; zz <= wZ; zz++)
		{
			for (int yy = rPY; yy > rPY-deep; yy--)
			{
				if ((xx == -wX && zz ==  wZ) ||
					(xx ==  wX && zz ==  wZ) ||
					(xx ==  wX && zz == -wZ) ||
					(xx == -wX && zz == -wZ)) continue;
				chunk->removeBlock(grid,sf::Vector3i(X+xx,yy,Z+zz));
			}
		}
		*/
	}
}

void Terrain::createSea(Chunk *chunk, Grid grid, int noiseHeight, int x, int z)
{
	for (int yy = 16; yy < SEA_LEVEL + 2; ++yy)
	{
		if (grid.getValueAt(sf::Vector3i(x,yy,z)) == Types::AIR)
		{
			chunk->addBlock(grid,sf::Vector3i(x,yy,z),Types::WATER);

			if (grid.getValueAt(sf::Vector3i(x,yy-1,z)) == Types::GRASS)
			{
				UInt seed = static_cast<UInt>(Math::random(4));

				chunk->removeBlock(grid,sf::Vector3i(x,yy-1,z));
				if (seed == 3) chunk->addBlock(grid,sf::Vector3i(x,yy-1,z),Types::STONE);
				else chunk->addBlock(grid,sf::Vector3i(x,yy-1,z),Types::DIRT);
			}
		}
	}
}

void Terrain::createTrees(Chunk *chunk, Grid grid, int noiseHeight, int x, int z)
{
	if (grid.getValueAt(sf::Vector3i(x,noiseHeight-1,z)) != Types::GRASS ||
		grid.getValueAt(sf::Vector3i(x,noiseHeight,z)) != Types::AIR) return;
	int seeds = Math::random(100);
	if (seeds == 20)
	{
		int treeHeight = Math::randomRange(4,8);
		int leafShape = Math::random(3);
		switch (leafShape)
		{
			case 0:
			{
				for (int xx = -2; xx <= 2; ++xx)
				for (int zz = -2; zz <= 2; ++zz)
				{
					chunk->addBlock(grid,sf::Vector3i(x+xx,noiseHeight+treeHeight,z+zz),Types::LEAF);
					if (xx == -2 || zz == -2 ||
						xx ==  2 || zz ==  2) continue;
					chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+1,z+zz),Types::LEAF);
					if (xx == -1 || zz == -1 ||
						xx ==  1 || zz ==  1) continue;
					chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+2,z+zz),Types::LEAF);
				}
				break;
			}
			case 1:
			{
				for (int xx = -2; xx <= 2; ++xx)
				for (int zz = -2; zz <= 2; ++zz)
				{
					if ((xx == -2 && zz == -2) ||
						(xx == -2 && zz ==  2) ||
						(xx ==  2 && zz == -2) ||
						(xx ==  2 && zz ==  2)) continue;
					chunk->addBlock(grid,sf::Vector3i(x+xx,noiseHeight+treeHeight,z+zz),Types::LEAF);
					chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+1,z+zz),Types::LEAF);
					chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+2,z+zz),Types::LEAF);
				}
				break;
			}
			case 2:
			case 3:
			{
				for (int xx = -1; xx <= 1; ++xx)
				for (int zz = -1; zz <= 1; ++zz)
				{
					if ((xx == -1 && zz == -1) ||
						(xx == -1 && zz ==  1) ||
						(xx ==  1 && zz == -1) ||
						(xx ==  1 && zz ==  1)) continue;
					for (int yy = noiseHeight+treeHeight; yy <= (noiseHeight+treeHeight)+5; yy++)
						chunk->addBlock(grid,sf::Vector3i(x+xx,yy,z+zz),Types::LEAF);
					if (xx == 0 && zz == 0)
					{
						chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+6,z+zz),Types::LEAF);
						chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+7,z+zz),Types::LEAF);
						chunk->addBlock(grid,sf::Vector3i(x+xx,(noiseHeight+treeHeight)+8,z+zz),Types::LEAF);
					}
				}
				break;
			}
		}
		int trunkHeight = (noiseHeight + treeHeight) + 2;
		for (int y1 = noiseHeight; y1 < trunkHeight; y1++)
		{
			chunk->addBlock(grid,sf::Vector3i(x,y1,z),Types::LOG);
		}
	}
}

void Terrain::createBushes(Chunk *chunk, Grid grid, int noiseHeight, int x, int z)
{
	UInt tallgrassSeed = static_cast<UInt>(Math::random(5));

	if (tallgrassSeed == 3)
	{
		if ((grid.getValueAt(sf::Vector3i(x,noiseHeight-1,z)) == Types::GRASS ||
			 grid.getValueAt(sf::Vector3i(x,noiseHeight-1,z)) == Types::DIRT) &&
			 grid.getValueAt(sf::Vector3i(x,noiseHeight,z)) == Types::AIR)
		{
			chunk->addBlock(grid,sf::Vector3i(x,noiseHeight,z),Types::TALLGRASS);
		}
	}

	tallgrassSeed = static_cast<UInt>(Math::random(5));

	if (tallgrassSeed == 3)
	{
		if (grid.getValueAt(sf::Vector3i(x,noiseHeight,z)) == Types::TALLGRASS)
			chunk->addBlock(grid,sf::Vector3i(x,noiseHeight+1,z),Types::TALLGRASS);
	}

	UInt roseSeed = static_cast<UInt>(Math::random(100));

	if (roseSeed == 2)
	{
		if ((grid.getValueAt(sf::Vector3i(x,noiseHeight-1,z)) == Types::GRASS ||
			 grid.getValueAt(sf::Vector3i(x,noiseHeight-1,z)) == Types::DIRT) &&
			 grid.getValueAt(sf::Vector3i(x,noiseHeight,z)) == Types::AIR &&
			 grid.getValueAt(sf::Vector3i(x,noiseHeight,z)) != Types::TALLGRASS)
		{
			chunk->addBlock(grid,sf::Vector3i(x,noiseHeight,z),Types::ROSE);
		}
	}
}


/* PUBLIC FUNCTION */


void Terrain::generate(Chunk* chunk, Grid grid, Noise *noise)
{
	int sizeX = grid.getSize().x;
	int sizeZ = grid.getSize().z;

	for (int s = 0; s < sizeX * sizeZ; ++s)
	{
		noiseHeight = static_cast<UInt>((noise->getHeight(s / sizeX, s % sizeZ) * 48) + MAX_NOISE_HEIGHT);

		createLand(chunk, grid, noiseHeight, s / sizeX, s % sizeZ);
		createCave(chunk, grid, noiseHeight, s / sizeX, s % sizeZ);
		createSea(chunk, grid, noiseHeight, s / sizeX, s % sizeZ);
		createTrees(chunk, grid, noiseHeight, s / sizeX, s % sizeZ);
		createBushes(chunk, grid, noiseHeight, s / sizeX, s % sizeZ);
	}
}

UInt Terrain::getNoiseHeight()
{
	return noiseHeight;
}


