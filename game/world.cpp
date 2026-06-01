
#include "world.h"

World::World(int seed)
{
	renderDistance = 8; // 8 chunks (normal)
	updateDistance = 24;

	fov = near = far = 0.0f;

	worldMap.createGrid(sf::Vector3i(WORLD_SIZE_H,WORLD_SIZE_V,WORLD_SIZE_H),8); // we only store 0 - 255 blocks

	chunk = new Chunk(worldMap);

	Math::initRandom();

	if (seed == 0)
	{
		seed = Math::randomRange(100000,9999999);
	}

	this->seed = seed;

	perlinNoise = new Noise(seed);

	terrain.generate(chunk,worldMap,perlinNoise);
}

World::~World()
{
	delete chunk;
	delete perlinNoise;

	worldMap.destroyGrid();
}

void World::render(sf::Vector3f position, Raycast raycast)
{
	skybox.setSize(skybox.getDefaultSize() * (renderDistance+3));
	skybox.render(sf::Vector3i(position.x,position.y,position.z));

	clouds.render(sf::Vector3i(position.x,position.y,position.z));

	size = static_cast<float>(CHUNK_SIZE*BLOCK_SIZE);
	radius = sqrt(Math::sqr(size/2) * (2 + (fov/100)));

	X1 = (ceil(position.x / CHUNK_SIZE) / BLOCK_SIZE) - renderDistance;
	Y1 = (ceil(position.y / CHUNK_SIZE) / BLOCK_SIZE) - renderDistance;
	Z1 = (ceil(position.z / CHUNK_SIZE) / BLOCK_SIZE) - renderDistance;

	X2 = (ceil(position.x / CHUNK_SIZE) / BLOCK_SIZE) + renderDistance;
	Y2 = (ceil(position.y / CHUNK_SIZE) / BLOCK_SIZE) + renderDistance;
	Z2 = (ceil(position.z / CHUNK_SIZE) / BLOCK_SIZE) + renderDistance;

	for (int y = Y1; y <= Y2; ++y)
	for (int x = X1; x <= X2; ++x)
	for (int z = Z1; z <= Z2; ++z)
	{
		if (!raycast.surfaceHidden(sf::Vector3f(x*size+size/2,y*size+size/2,z*size+size/2),radius)) continue;
		chunk->drawUnBlendedChunk(worldMap,sf::Vector3i(x,y,z));
	}

	for (int y = Y1; y <= Y2; ++y)
	for (int x = X1; x <= X2; ++x)
	for (int z = Z1; z <= Z2; ++z)
	{
		if (!raycast.surfaceHidden(sf::Vector3f(x*size+size/2,y*size+size/2,z*size+size/2),radius)) continue;
		chunk->drawBlendedChunk(worldMap,sf::Vector3i(x,y,z));
	}

	blockPhysics(worldMap,sf::Vector3i(position));
}

int World::getRenderDistance()
{
	return renderDistance;
}

Grid World::getMap()
{
	return worldMap;
}

Chunk* World::getChunk()
{
	return chunk;
}

void World::setViewProperty(sf::Vector3f props)
{
	fov = props.x;
	near = props.y;
	far = props.z;
}

UInt World::getSeed()
{
	return seed;
}

UInt World::getNoiseHeight()
{
	return terrain.getNoiseHeight();
}

/* -- ISOLATED FUNCTIONS -- */


void World::blockPhysics(Grid grid, sf::Vector3i position)
{
	tick[0].setTickSpeed(DEFAULT_TICK_SPEED / 2);

	if (tick[0].wait()) return;

	tick[1].setTickSpeed((DEFAULT_TICK_SPEED+1) - DEFAULT_TICK_SPEED);
	tick[2].setTickSpeed(DEFAULT_TICK_SPEED + 2);
	tick[3].setTickSpeed(DEFAULT_TICK_SPEED * Math::randomRange(50,70));

	XX = (position.x / BLOCK_SIZE);
	YY = (position.y / BLOCK_SIZE);
	ZZ = (position.z / BLOCK_SIZE);

	for (int y = YY - updateDistance; y <= YY + updateDistance; ++y)
	for (int x = XX - updateDistance; x <= XX + updateDistance; ++x)
	for (int z = ZZ - updateDistance; z <= ZZ + updateDistance; ++z)
	{
		if (y == 0 || x == 0 || z == 0 ||
			y == grid.getSize().y-1 || x == grid.getSize().x-1 || z == grid.getSize().z-1) continue;

		UInt blok = grid.getValueAt(sf::Vector3i(x,y,z));

		if (blok == Types::AIR) continue;

		if (blok == Types::SAND)
		{
			if (!Types::checkSolids(grid.getValueAt(sf::Vector3i(x,y-1,z))))
			{
				chunk->removeBlock(grid,sf::Vector3i(x,y,z));
				chunk->addBlock(grid,sf::Vector3i(x,y-1,z),Types::SAND);
			}
		}
		else if (blok == Types::WATER)
		{
			if (!Types::checkSolids(grid.getValueAt(sf::Vector3i(x,y-1,z))) &&
				!Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y-1,z)))) {
				if (tick[1].wait()) continue;
				chunk->addBlock(grid,sf::Vector3i(x,y-1,z),Types::WATER);
				tick[1].reset();
			}

			if (Types::checkSolids(grid.getValueAt(sf::Vector3i(x,y-1,z))))
			{
				if 		(!Types::checkSolids(grid.getValueAt(sf::Vector3i(x-1,y,z))) &&
						 !Types::checkLiquids(grid.getValueAt(sf::Vector3i(x-1,y,z)))) {
					if (tick[1].wait()) continue;
					chunk->addBlock(grid,sf::Vector3i(x-1,y,z),Types::WATER);
					tick[1].reset();
				}
				else if (!Types::checkSolids(grid.getValueAt(sf::Vector3i(x+1,y,z))) &&
						 !Types::checkLiquids(grid.getValueAt(sf::Vector3i(x+1,y,z)))) {
					if (tick[1].wait()) continue;
					chunk->addBlock(grid,sf::Vector3i(x+1,y,z),Types::WATER);
					tick[1].reset();
				}
				else if (!Types::checkSolids(grid.getValueAt(sf::Vector3i(x,y,z-1))) &&
						 !Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y,z-1)))) {
					if (tick[1].wait()) continue;
					chunk->addBlock(grid,sf::Vector3i(x,y,z-1),Types::WATER);
					tick[1].reset();
				}
				else if (!Types::checkSolids(grid.getValueAt(sf::Vector3i(x,y,z+1))) &&
						 !Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y,z+1)))) {
					if (tick[1].wait()) continue;
					chunk->addBlock(grid,sf::Vector3i(x,y,z+1),Types::WATER);
					tick[1].reset();
				}
			}
		}
		else if (blok == Types::LAVA)
		{
			if (!Types::checkSolids(grid.getValueAt(sf::Vector3i(x,y-1,z))) &&
				!Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y-1,z)))) {
				if (tick[2].wait()) continue;
				chunk->addBlock(grid,sf::Vector3i(x,y-1,z),Types::LAVA);
				tick[2].reset();
			}

			if (Types::checkSolids(grid.getValueAt(sf::Vector3i(x,y-1,z))))
			{
				if 		(!Types::checkSolids(grid.getValueAt(sf::Vector3i(x-1,y,z))) &&
						 !Types::checkLiquids(grid.getValueAt(sf::Vector3i(x-1,y,z)))) {
					if (tick[2].wait()) continue;
					chunk->addBlock(grid,sf::Vector3i(x-1,y,z),Types::LAVA);
					tick[2].reset();
				}
				else if (!Types::checkSolids(grid.getValueAt(sf::Vector3i(x+1,y,z))) &&
						 !Types::checkLiquids(grid.getValueAt(sf::Vector3i(x+1,y,z)))) {
					if (tick[2].wait()) continue;
					chunk->addBlock(grid,sf::Vector3i(x+1,y,z),Types::LAVA);
					tick[2].reset();
				}
				else if (!Types::checkSolids(grid.getValueAt(sf::Vector3i(x,y,z-1))) &&
						 !Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y,z-1)))) {
					if (tick[2].wait()) continue;
					chunk->addBlock(grid,sf::Vector3i(x,y,z-1),Types::LAVA);
					tick[2].reset();
				}
				else if (!Types::checkSolids(grid.getValueAt(sf::Vector3i(x,y,z+1))) &&
						 !Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y,z+1)))) {
					if (tick[2].wait()) continue;
					chunk->addBlock(grid,sf::Vector3i(x,y,z+1),Types::LAVA);
					tick[2].reset();
				}
			}
		}
		else if (Types::checkBush(blok))
		{
			if (blok != Types::TALLGRASS)
			{
				if (grid.getValueAt(sf::Vector3i(x,y-1,z)) != Types::GRASS &&
					grid.getValueAt(sf::Vector3i(x,y-1,z)) != Types::DIRT)
					chunk->removeBlock(grid,sf::Vector3i(x,y,z));
				else if (Types::checkBush(grid.getValueAt(sf::Vector3i(x,y-1,z))))
					chunk->removeBlock(grid,sf::Vector3i(x,y,z));
			}
			else
			{
				if (grid.getValueAt(sf::Vector3i(x,y-1,z)) != Types::GRASS &&
					grid.getValueAt(sf::Vector3i(x,y-1,z)) != Types::DIRT &&
					grid.getValueAt(sf::Vector3i(x,y-1,z)) != Types::TALLGRASS)
					chunk->removeBlock(grid,sf::Vector3i(x,y,z));
				if (grid.getValueAt(sf::Vector3i(x,y-1,z)) == Types::TALLGRASS &&
					grid.getValueAt(sf::Vector3i(x,y-2,z)) == Types::TALLGRASS)
					chunk->removeBlock(grid,sf::Vector3i(x,y,z));
			}
		}
		else if (blok == Types::DIRT)
		{
			if (grid.getValueAt(sf::Vector3i(x,y+1,z)) == Types::AIR ||
				Types::checkBush(grid.getValueAt(sf::Vector3i(x,y+1,z))))
			{
				if (tick[3].wait()) continue;
				chunk->removeBlock(grid,sf::Vector3i(x,y,z));
				chunk->addBlock(grid,sf::Vector3i(x,y,z),Types::GRASS);
				tick[3].reset();
			}
		}
	}

	tick[0].reset();
}

