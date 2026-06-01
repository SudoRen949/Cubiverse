
#include "chunk.h"

Chunk::Chunk(Grid grid)
{
	texClass.load("assets/textures/blocks.png");

	int cx = grid.getSize().x / CHUNK_SIZE;
	int cy = grid.getSize().y / CHUNK_SIZE;
	int cz = grid.getSize().z / CHUNK_SIZE;

	for (int i = 0; i < 2; i++)
	{
		buffer[i].createGrid(sf::Vector3i(cx,cy,cz),16);
		update[i].createGrid(sf::Vector3i(cx,cy,cz));
	}

	updateCount = 0;
	mergedTexure = false;
}

Chunk::~Chunk()
{
	for (int i = 0; i < 2; i++)
	{
		for (int y = 0; y < buffer[i].getSize().y; y++)
		for (int x = 0; x < buffer[i].getSize().x; x++)
		for (int z = 0; z < buffer[i].getSize().z; z++)
		{
			glDeleteLists(buffer[i].getValueAt(sf::Vector3i(x,y,z)),1);
		}

		buffer[i].destroyGrid();
		update[i].destroyGrid();
	}
}

void Chunk::addBlock(Grid grid, sf::Vector3i position, int block)
{
	if (block == Types::AIR) return;
	grid.setValueAt(position,block);
	updateNearChunk(position);
}

void Chunk::removeBlock(Grid grid, sf::Vector3i position)
{
	if (grid.getValueAt(position) == Types::AIR) return;
	grid.setValueAt(position,Types::AIR);
	updateNearChunk(position);
}

void Chunk::drawUnBlendedChunk(Grid grid, sf::Vector3i position)
{
	if (updateCounter > 0)
	{
		timer.wait(1500);
		if (timer.reached())
		{
			updateCounter = updateCount = 0;
			timer.reset(false);
		}
	}

	if (update[0].getValueAt(position) == 1)
	{
		buffer[0].setValueAt(position,glGenLists(1));

		glNewList(buffer[0].getValueAt(position),GL_COMPILE);

		initOpaqueChunk(grid,position);
		initTransparentChunk(grid, position);

		glEndList();

		update[0].setValueAt(position,0);
	}

	if (buffer[0].getValueAt(position) > 0) glCallList(buffer[0].getValueAt(position));

	if (updateCounter > 0)
	{
		if ((updateCounter % 2) == 0)
			updateCount = updateCounter / 2;
	}
}

void Chunk::drawBlendedChunk(Grid grid, sf::Vector3i position)
{
	if (updateCounter > 0)
	{
		timer.wait(1500);
		if (timer.reached())
		{
			updateCounter = updateCount = 0;
			timer.reset(false);
		}
	}

	if (update[1].getValueAt(position) == 1)
	{
		buffer[1].setValueAt(position,glGenLists(1));

		glNewList(buffer[1].getValueAt(position),GL_COMPILE);

		initBlendedChunk(grid,position);

		glEndList();

		updateCounter++;

		update[1].setValueAt(position,0);
	}

	if (buffer[1].getValueAt(position) > 0) glCallList(buffer[1].getValueAt(position));

	if (updateCounter > 0)
	{
		if ((updateCounter % 2) == 0)
			updateCount = updateCounter / 2;
	}
}

UInt Chunk::getUpdateCount()
{
	return (UInt) updateCount;
}

void Chunk::connectedTexture(bool enable)
{
	mergedTexure = enable;
}



/* -- PRIVATE FUNCTIONS -- */



void Chunk::updateChunk(sf::Vector3i position)
{
	int bX, bY, bZ;

	bX = update[0].getSize().x;
	bY = update[0].getSize().y;
	bZ = update[0].getSize().z;

	if (position.x < 0 || position.y < 0 || position.z < 0) return;
	if (position.x >= bX || position.y >= bY || position.z >= bZ) return;

	for (int i = 0; i < 2; i++)
	{
		if (update[i].getValueAt(position) == 0) update[i].setValueAt(position,1);
	}
}

void Chunk::updateNearChunk(sf::Vector3i position)
{
	UShort cx, cy, cz;

	/* What chunk does the block at? */

	cx = position.x / CHUNK_SIZE;
	cy = position.y / CHUNK_SIZE;
	cz = position.z / CHUNK_SIZE;

	updateChunk(sf::Vector3i(cx,cy,cz)); // update current chunk

	/* Update neighboring chunks (if the block is at the edge of the chunk) */

	if (position.y == (((position.y+1)/CHUNK_SIZE)*CHUNK_SIZE)-1) 	updateChunk(sf::Vector3i(cx,cy+1,cz));
	if (position.x == (((position.x+1)/CHUNK_SIZE)*CHUNK_SIZE)-1) 	updateChunk(sf::Vector3i(cx+1,cy,cz));
	if (position.z == (((position.z+1)/CHUNK_SIZE)*CHUNK_SIZE)-1) 	updateChunk(sf::Vector3i(cx,cy,cz+1));
	if (position.y == (((position.y+1)/CHUNK_SIZE)*CHUNK_SIZE)) 	updateChunk(sf::Vector3i(cx,cy-1,cz));
	if (position.x == (((position.x+1)/CHUNK_SIZE)*CHUNK_SIZE)) 	updateChunk(sf::Vector3i(cx-1,cy,cz));
	if (position.z == (((position.z+1)/CHUNK_SIZE)*CHUNK_SIZE)) 	updateChunk(sf::Vector3i(cx,cy,cz-1));
}





/* -- ISOLATED FUNCTIONS */





void Chunk::initOpaqueChunk(Grid grid, sf::Vector3i position)
{
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	glEnable(GL_CULL_FACE);

	texClass.bind();

	glBegin(GL_QUADS);

	int Y1 = position.y * CHUNK_SIZE;
	int X1 = position.x * CHUNK_SIZE;
	int Z1 = position.z * CHUNK_SIZE;

	int Y2 = CHUNK_SIZE + (position.y * CHUNK_SIZE);
	int X2 = CHUNK_SIZE + (position.x * CHUNK_SIZE);
	int Z2 = CHUNK_SIZE + (position.z * CHUNK_SIZE);

	for (int y = Y1; y < Y2; y++)
	for (int x = X1; x < X2; x++)
	for (int z = Z1; z < Z2; z++)
	{
		if (x >= grid.getSize().x) break;
		if (y >= grid.getSize().y) break;
		if (z >= grid.getSize().z) break;

		UShort block = grid.getValueAt(sf::Vector3i(x,y,z));

		if (block == Types::AIR || !Types::checkOpaque(block)) continue;
		else
		{
			x1 = x * BLOCK_SIZE;	x2 = (x+1) * BLOCK_SIZE;
			y1 = y * BLOCK_SIZE;	y2 = (y+1) * BLOCK_SIZE;
			z1 = z * BLOCK_SIZE;	z2 = (z+1) * BLOCK_SIZE;

			texClass.calculateHMap(block);

			drawOpaqueFaces(grid,position,x,y,z,block);
		}
	}

	glEnd();
}

void Chunk::initTransparentChunk(Grid grid, sf::Vector3i position)
{
	glEnable(GL_ALPHA_TEST);

	glAlphaFunc(GL_GEQUAL,0.5f);

	glDisable(GL_BLEND);

	texClass.bind();

	glEnable(GL_CULL_FACE);

	glBegin(GL_QUADS);

	int Y1 = position.y * CHUNK_SIZE;
	int X1 = position.x * CHUNK_SIZE;
	int Z1 = position.z * CHUNK_SIZE;

	int Y2 = CHUNK_SIZE + (position.y * CHUNK_SIZE);
	int X2 = CHUNK_SIZE + (position.x * CHUNK_SIZE);
	int Z2 = CHUNK_SIZE + (position.z * CHUNK_SIZE);

	for (int y = Y1; y < Y2; y++)
	for (int x = X1; x < X2; x++)
	for (int z = Z1; z < Z2; z++)
	{
		if (x >= grid.getSize().x) break;
		if (y >= grid.getSize().y) break;
		if (z >= grid.getSize().z) break;

		UShort block = grid.getValueAt(sf::Vector3i(x,y,z));

		if (block == Types::AIR || !Types::checkTransparent(block)) continue;
		else
		{
			if (!Types::checkBush(block))
			{
				x1 = x * BLOCK_SIZE;	x2 = (x+1) * BLOCK_SIZE;
				y1 = y * BLOCK_SIZE;	y2 = (y+1) * BLOCK_SIZE;
				z1 = z * BLOCK_SIZE;	z2 = (z+1) * BLOCK_SIZE;

				texClass.calculateHMap(block);

				drawTransparentFaces(grid,position,x,y,z,block);
			}
		}
	}

	glEnd();

	glDisable(GL_CULL_FACE);

	glBegin(GL_QUADS);

	for (int y = Y1; y < Y2; y++)
	for (int x = X1; x < X2; x++)
	for (int z = Z1; z < Z2; z++)
	{
		if (x >= grid.getSize().x) break;
		if (y >= grid.getSize().y) break;
		if (z >= grid.getSize().z) break;

		UShort block = grid.getValueAt(sf::Vector3i(x,y,z));

		if (block == Types::AIR || !Types::checkTransparent(block)) continue;
		else
		{
			if (Types::checkBush(block))
			{
				x1 = x * BLOCK_SIZE;	x2 = (x+1) * BLOCK_SIZE;
				y1 = y * BLOCK_SIZE;	y2 = (y+1) * BLOCK_SIZE;
				z1 = z * BLOCK_SIZE;	z2 = (z+1) * BLOCK_SIZE;

				texClass.calculateHMap(block);

				drawBushesFaces(grid,position,x,y,z,block);
			}
		}
	}

	glEnd();
}

void Chunk::initBlendedChunk(Grid grid, sf::Vector3i position)
{
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glDisable(GL_ALPHA_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	texClass.bind();

	glBegin(GL_QUADS);

	for (int y = position.y * CHUNK_SIZE; y < CHUNK_SIZE + (position.y * CHUNK_SIZE); y++)
	for (int x = position.x * CHUNK_SIZE; x < CHUNK_SIZE + (position.x * CHUNK_SIZE); x++)
	for (int z = position.z * CHUNK_SIZE; z < CHUNK_SIZE + (position.z * CHUNK_SIZE); z++)
	{
		if (x >= grid.getSize().x) break;
		if (y >= grid.getSize().y) break;
		if (z >= grid.getSize().z) break;

		UShort block = grid.getValueAt(sf::Vector3i(x,y,z));

		if (block == Types::AIR || !Types::checkBlend(block)) continue;
		else {
			x1 = x * BLOCK_SIZE;	x2 = (x+1) * BLOCK_SIZE;
			y1 = y * BLOCK_SIZE;	y2 = (y+1) * BLOCK_SIZE;
			z1 = z * BLOCK_SIZE;	z2 = (z+1) * BLOCK_SIZE;
			texClass.calculateHMap(block);
			drawBlendedFaces(grid,position,x,y,z,block);
		}
	}

	glEnd();
}

void Chunk::drawOpaqueFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block)
{
	top = grid.getValueAt(sf::Vector3i(x,y+1,z));
	bottom = grid.getValueAt(sf::Vector3i(x,y-1,z));
	left = grid.getValueAt(sf::Vector3i(x-1,y,z));
	right = grid.getValueAt(sf::Vector3i(x+1,y,z));
	front = grid.getValueAt(sf::Vector3i(x,y,z+1));
	back = grid.getValueAt(sf::Vector3i(x,y,z-1));

	if (top == Types::AIR || Types::checkNotOpaque(top) || y >= grid.getSize().y-1)
	{
		glColor3f(1,1,1);

		glTexCoord2f(texClass.getMap(1,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(1,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(1,4),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(1,6),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2,z1);
	}

	if (bottom == Types::AIR || Types::checkNotOpaque(bottom) || y == 0)
	{
		glColor3f(0.5,0.5,0.5);

		glTexCoord2f(texClass.getMap(3,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1,z1);
		glTexCoord2f(texClass.getMap(3,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1,z1);
		glTexCoord2f(texClass.getMap(3,4),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y1,z2);
		glTexCoord2f(texClass.getMap(3,6),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y1,z2);
	}

	if (right == Types::AIR || Types::checkNotOpaque(right) || x > grid.getSize().x)
	{
		glColor3f(0.8,0.8,0.8);
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y1,z1);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1,z2);
	}

	if (left == Types::AIR || Types::checkNotOpaque(left) || x < 0)
	{
		glColor3f(0.7,0.7,0.7);
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1,z2);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y1,z1);
	}

	if (front == Types::AIR || Types::checkNotOpaque(front) || z > grid.getSize().z)
	{
		glColor3f(0.6,0.6,0.6);
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y1,z2);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y1,z2);
	}

	if (back == Types::AIR || Types::checkNotOpaque(back) || z < 0)
	{
		glColor3f(0.5,0.5,0.5);
		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1,z1);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1,z1);
	}
}

void Chunk::drawTransparentFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block)
{
	float l1 = 0, l2 = 0, l3 = 0, l4 = 0;
	float f1 = 0, f2 = 0, f3 = 0, f4 = 0;

	top = grid.getValueAt(sf::Vector3i(x,y+1,z));
	bottom = grid.getValueAt(sf::Vector3i(x,y-1,z));
	left = grid.getValueAt(sf::Vector3i(x-1,y,z));
	right = grid.getValueAt(sf::Vector3i(x+1,y,z));
	front = grid.getValueAt(sf::Vector3i(x,y,z+1));
	back = grid.getValueAt(sf::Vector3i(x,y,z-1));

	if (Types::checkBlend(top) || Types::checkOpaque(top) || Types::checkBush(top) || y >= grid.getSize().y-1)
	{
		if (mergedTexure)
		{
			l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;

			if (Types::checkTransparent(left)) 	{ l1 = 0.005; l4 = 0.005; }
			if (Types::checkTransparent(right)) { l2 = 0.005; l3 = 0.005; }
			if (Types::checkTransparent(back)) 	{ f1 = 0.005; f4 = 0.005; }
			if (Types::checkTransparent(front)) { f2 = 0.005; f3 = 0.005; }
		}

		glColor3f(1,1,1);

		glTexCoord2f(texClass.getMap(1,0)+l1,texClass.getMap(block%texClass.getNBlocks().y,1)-f3); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(1,2)-l2,texClass.getMap(block%texClass.getNBlocks().y,3)-f2); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(1,4)-l3,texClass.getMap(block%texClass.getNBlocks().y,5)+f1); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(1,6)+l4,texClass.getMap(block%texClass.getNBlocks().y,7)+f4); glVertex3i(x1,y2,z1);
	}

	if (Types::checkBlend(bottom) || Types::checkOpaque(bottom) || Types::checkBush(bottom) || y == 0)
	{
		if (mergedTexure)
		{
			l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;

			if (Types::checkTransparent(left)) 	{ l1 = 0.005; l4 = 0.005; }
			if (Types::checkTransparent(right)) { l2 = 0.005; l3 = 0.005; }
			if (Types::checkTransparent(back)) 	{ f1 = 0.005; f4 = 0.005; }
			if (Types::checkTransparent(front)) { f2 = 0.005; f3 = 0.005; }
		}

		glColor3f(0.5,0.5,0.5);

		glTexCoord2f(texClass.getMap(3,0)+l1,texClass.getMap(block%texClass.getNBlocks().y,1)-f1); glVertex3i(x1,y1,z1);
		glTexCoord2f(texClass.getMap(3,2)-l2,texClass.getMap(block%texClass.getNBlocks().y,3)-f4); glVertex3i(x2,y1,z1);
		glTexCoord2f(texClass.getMap(3,4)-l3,texClass.getMap(block%texClass.getNBlocks().y,5)+f3); glVertex3i(x2,y1,z2);
		glTexCoord2f(texClass.getMap(3,6)+l4,texClass.getMap(block%texClass.getNBlocks().y,7)+f2); glVertex3i(x1,y1,z2);
	}

	if (Types::checkBlend(right) || Types::checkOpaque(right) || Types::checkBush(right) || x > grid.getSize().x)
	{
		if (mergedTexure)
		{
			l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;

			if (Types::checkTransparent(bottom)) 	{ l1 = 0.005; l4 = 0.005; }
			if (Types::checkTransparent(top)) 		{ l2 = 0.005; l3 = 0.005; }
			if (Types::checkTransparent(back)) 		{ f1 = 0.005; f4 = 0.005; }
			if (Types::checkTransparent(front)) 	{ f2 = 0.005; f3 = 0.005; }
		}

		glColor3f(0.8,0.8,0.8);

		glTexCoord2f(texClass.getMap(2,0)+f1,texClass.getMap(block%texClass.getNBlocks().y,1)-l1); glVertex3i(x2,y1,z1);
		glTexCoord2f(texClass.getMap(2,6)+f4,texClass.getMap(block%texClass.getNBlocks().y,5)+l2); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,4)-f2,texClass.getMap(block%texClass.getNBlocks().y,7)+l3); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,2)-f3,texClass.getMap(block%texClass.getNBlocks().y,3)-l4); glVertex3i(x2,y1,z2);
	}

	if (Types::checkBlend(left) || Types::checkOpaque(left) || Types::checkBush(left) || x < 0)
	{
		if (mergedTexure)
		{
			l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;

			if (Types::checkTransparent(bottom)) 	{ l1 = 0.005; l4 = 0.005; }
			if (Types::checkTransparent(top)) 		{ l2 = 0.005; l3 = 0.005; }
			if (Types::checkTransparent(back)) 		{ f1 = 0.005; f4 = 0.005; }
			if (Types::checkTransparent(front)) 	{ f2 = 0.005; f3 = 0.005; }
		}

		glColor3f(0.7,0.7,0.7);

		glTexCoord2f(texClass.getMap(2,0)+f2,texClass.getMap(block%texClass.getNBlocks().y,1)-l1); glVertex3i(x1,y1,z2);
		glTexCoord2f(texClass.getMap(2,6)+f3,texClass.getMap(block%texClass.getNBlocks().y,5)+l2); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,4)-f1,texClass.getMap(block%texClass.getNBlocks().y,7)+l3); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,2)-f4,texClass.getMap(block%texClass.getNBlocks().y,3)-l4); glVertex3i(x1,y1,z1);
	}

	if (Types::checkBlend(front) || Types::checkOpaque(front) || Types::checkBush(front) || z > grid.getSize().z)
	{
		if (mergedTexure)
		{
			l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;

			if (Types::checkTransparent(bottom)) 	{ l1 = 0.005; l4 = 0.005; }
			if (Types::checkTransparent(top)) 		{ l2 = 0.005; l3 = 0.005; }
			if (Types::checkTransparent(left)) 		{ f1 = 0.005; f4 = 0.005; }
			if (Types::checkTransparent(right)) 	{ f2 = 0.005; f3 = 0.005; }
		}

		glColor3f(0.6,0.6,0.6);

		glTexCoord2f(texClass.getMap(2,0)+f2,texClass.getMap(block%texClass.getNBlocks().y,1)-l1); glVertex3i(x2,y1,z2);
		glTexCoord2f(texClass.getMap(2,6)+f3,texClass.getMap(block%texClass.getNBlocks().y,5)+l2); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,4)-f1,texClass.getMap(block%texClass.getNBlocks().y,7)+l3); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,2)-f4,texClass.getMap(block%texClass.getNBlocks().y,3)-l4); glVertex3i(x1,y1,z2);
	}

	if (Types::checkBlend(back) || Types::checkOpaque(back) || Types::checkBush(back) || z < 0)
	{
		if (mergedTexure)
		{
			l1 = l2 = l3 = l4 = f1 = f2 = f3 = f4 = 0;

			if (Types::checkTransparent(bottom)) 	{ l1 = 0.005; l4 = 0.005; }
			if (Types::checkTransparent(top)) 		{ l2 = 0.005; l3 = 0.005; }
			if (Types::checkTransparent(left)) 		{ f1 = 0.005; f4 = 0.005; }
			if (Types::checkTransparent(right)) 	{ f2 = 0.005; f3 = 0.005; }
		}

		glColor3f(0.5,0.5,0.5);

		glTexCoord2f(texClass.getMap(2,0)+f1,texClass.getMap(block%texClass.getNBlocks().y,1)-l1); glVertex3i(x1,y1,z1);
		glTexCoord2f(texClass.getMap(2,6)+f4,texClass.getMap(block%texClass.getNBlocks().y,5)+l2); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,4)-f2,texClass.getMap(block%texClass.getNBlocks().y,7)+l3); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,2)-f3,texClass.getMap(block%texClass.getNBlocks().y,3)-l4); glVertex3i(x2,y1,z1);
	}
}

void Chunk::drawBlendedFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block)
{
	short sy = 0;

	top 	= grid.getValueAt(sf::Vector3i(x,y+1,z));
	bottom 	= grid.getValueAt(sf::Vector3i(x,y-1,z));
	left 	= grid.getValueAt(sf::Vector3i(x-1,y,z));
	right 	= grid.getValueAt(sf::Vector3i(x+1,y,z));
	front 	= grid.getValueAt(sf::Vector3i(x,y,z+1));
	back 	= grid.getValueAt(sf::Vector3i(x,y,z-1));

	if (Types::checkLiquids(block) && (Types::checkOpaque(top) || Types::checkTransparent(top))) y2 -= 2;

	if (top == Types::AIR || Types::checkOpaque(top) || Types::checkTransparent(top) || y >= grid.getSize().y-1)
	{
		glColor3f(1,1,1);

		glTexCoord2f(texClass.getMap(1,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(1,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(1,4),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(1,6),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2,z1);

		/* bottom face */

		glTexCoord2f(texClass.getMap(1,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(1,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(1,4),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(1,6),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y2,z1);
	}

	if (bottom == Types::AIR || Types::checkTransparent(bottom) || y == 0)
	{
		glColor3f(0.5,0.5,0.5);

		glTexCoord2f(texClass.getMap(3,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1,z1);
		glTexCoord2f(texClass.getMap(3,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1,z1);
		glTexCoord2f(texClass.getMap(3,4),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y1,z2);
		glTexCoord2f(texClass.getMap(3,6),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y1,z2);

		if (!Types::checkTransparent(bottom))
		{
			glTexCoord2f(texClass.getMap(3,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y1,z1);
			glTexCoord2f(texClass.getMap(3,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y1,z1);
			glTexCoord2f(texClass.getMap(3,4),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y1,z2);
			glTexCoord2f(texClass.getMap(3,6),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y1,z2);
		}
	}

	if (right == Types::AIR || Types::checkTransparent(right) || x > grid.getSize().x)
	{
		if (Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y,z))) &&
			Types::checkLiquids(grid.getValueAt(sf::Vector3i(x+1,y-1,z))) &&
			Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y-1,z)))) sy = -2;

		glColor3f(0.8,0.8,0.8);

		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y1+sy,z1);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1+sy,z2);

		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y1+sy,z2);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1+sy,z1);
	}

	if (left == Types::AIR || Types::checkTransparent(left) || x < 0)
	{
		if (Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y,z))) &&
			Types::checkLiquids(grid.getValueAt(sf::Vector3i(x-1,y-1,z))) &&
			Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y-1,z)))) sy = -2;

		glColor3f(0.7,0.7,0.7);

		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1+sy,z2);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y1+sy,z1);

		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1+sy,z1);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y1+sy,z2);
	}

	if (front == Types::AIR || Types::checkTransparent(front) || z > grid.getSize().z)
	{
		if (Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y,z))) &&
			Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y-1,z+1))) &&
			Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y-1,z)))) sy = -2;

		glColor3f(0.6,0.6,0.6);

		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y1+sy,z2);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y1+sy,z2);

		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1+sy,z2);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y2,z2);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y2,z2);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1+sy,z2);
	}

	if (back == Types::AIR || Types::checkTransparent(back) || z < 0)
	{
		if (Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y,z))) &&
			Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y-1,z-1))) &&
			Types::checkLiquids(grid.getValueAt(sf::Vector3i(x,y-1,z)))) sy = -2;

		glColor3f(0.5,0.5,0.5);

		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x1,y1+sy,z1);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x2,y1+sy,z1);

		glTexCoord2f(texClass.getMap(2,0),texClass.getMap(block%texClass.getNBlocks().y,1)); glVertex3i(x2,y1+sy,z1);
		glTexCoord2f(texClass.getMap(2,6),texClass.getMap(block%texClass.getNBlocks().y,5)); glVertex3i(x2,y2,z1);
		glTexCoord2f(texClass.getMap(2,4),texClass.getMap(block%texClass.getNBlocks().y,7)); glVertex3i(x1,y2,z1);
		glTexCoord2f(texClass.getMap(2,2),texClass.getMap(block%texClass.getNBlocks().y,3)); glVertex3i(x1,y1+sy,z1);
	}
}

void Chunk::drawBushesFaces(Grid grid, sf::Vector3i position, int x, int y, int z, int block)
{
	UInt blok = grid.getValueAt(sf::Vector3i(x,y,z));

	glColor3f(1,1,1);

	if (blok != Types::TALLGRASS)
	{
		/* first half */

		glTexCoord2f(texClass.getMap(1,0),texClass.getMap(block%texClass.getNBlocks().y,1));

		glVertex3f(x2-BLOCK_SIZE,y1,z1);

		glTexCoord2f(texClass.getMap(1,2),texClass.getMap(block%texClass.getNBlocks().y,3));

		glVertex3f(x2,y1,z2);

		glTexCoord2f(texClass.getMap(1,4),texClass.getMap(block%texClass.getNBlocks().y,5));

		glVertex3f(x2,y2,z2);

		glTexCoord2f(texClass.getMap(1,6),texClass.getMap(block%texClass.getNBlocks().y,7));

		glVertex3f(x2-BLOCK_SIZE,y2,z1);

		/* second half */

		glTexCoord2f(texClass.getMap(3,0),texClass.getMap(block%texClass.getNBlocks().y,1));

		glVertex3f(x1,y1,z2);

		glTexCoord2f(texClass.getMap(3,2),texClass.getMap(block%texClass.getNBlocks().y,3));

		glVertex3f(x1+BLOCK_SIZE,y1,z1);

		glTexCoord2f(texClass.getMap(3,4),texClass.getMap(block%texClass.getNBlocks().y,5));

		glVertex3f(x1+BLOCK_SIZE,y2,z1);

		glTexCoord2f(texClass.getMap(3,6),texClass.getMap(block%texClass.getNBlocks().y,7));

		glVertex3f(x1,y2,z2);
	}
	else
	{
		if 		(grid.getValueAt(sf::Vector3i(x,y-1,z)) == Types::TALLGRASS) return;
		else if (grid.getValueAt(sf::Vector3i(x,y+1,z)) == Types::TALLGRASS)
		{
			/* first half */

			glTexCoord2f(texClass.getMap(1,0),texClass.getMap(block%texClass.getNBlocks().y,1));

			glVertex3f(x2-BLOCK_SIZE,y1,z1);

			glTexCoord2f(texClass.getMap(1,2),texClass.getMap(block%texClass.getNBlocks().y,3));

			glVertex3f(x2,y1,z2);

			glTexCoord2f(texClass.getMap(1,4),texClass.getMap(block%texClass.getNBlocks().y,5));

			glVertex3f(x2,y2+BLOCK_SIZE,z2);

			glTexCoord2f(texClass.getMap(1,6),texClass.getMap(block%texClass.getNBlocks().y,7));

			glVertex3f(x2-BLOCK_SIZE,y2+BLOCK_SIZE,z1);

			/* second half */

			glTexCoord2f(texClass.getMap(3,0),texClass.getMap(block%texClass.getNBlocks().y,1));

			glVertex3f(x1,y1,z2);

			glTexCoord2f(texClass.getMap(3,2),texClass.getMap(block%texClass.getNBlocks().y,3));

			glVertex3f(x1+BLOCK_SIZE,y1,z1);

			glTexCoord2f(texClass.getMap(3,4),texClass.getMap(block%texClass.getNBlocks().y,5));

			glVertex3f(x1+BLOCK_SIZE,y2+BLOCK_SIZE,z1);

			glTexCoord2f(texClass.getMap(3,6),texClass.getMap(block%texClass.getNBlocks().y,7));

			glVertex3f(x1,y2+BLOCK_SIZE,z2);
		}
		else
		{
			/* first half */

			glTexCoord2f(texClass.getMap(1,0),texClass.getMap(block%texClass.getNBlocks().y,1));

			glVertex3f(x2-BLOCK_SIZE,y1,z1-1);

			glTexCoord2f(texClass.getMap(1,2),texClass.getMap(block%texClass.getNBlocks().y,3));

			glVertex3f(x2,y1,z2);

			glTexCoord2f(texClass.getMap(1,4),texClass.getMap(block%texClass.getNBlocks().y,5));

			glVertex3f(x2,y2,z2);

			glTexCoord2f(texClass.getMap(1,6),texClass.getMap(block%texClass.getNBlocks().y,7));

			glVertex3f(x2-BLOCK_SIZE,y2,z1);

			/* second half */

			glTexCoord2f(texClass.getMap(3,0),texClass.getMap(block%texClass.getNBlocks().y,1));

			glVertex3f(x1,y1,z2);

			glTexCoord2f(texClass.getMap(3,2),texClass.getMap(block%texClass.getNBlocks().y,3));

			glVertex3f(x1+BLOCK_SIZE,y1,z1);

			glTexCoord2f(texClass.getMap(3,4),texClass.getMap(block%texClass.getNBlocks().y,5));

			glVertex3f(x1+BLOCK_SIZE,y2,z1);

			glTexCoord2f(texClass.getMap(3,6),texClass.getMap(block%texClass.getNBlocks().y,7));

			glVertex3f(x1,y2,z2);
		}
	}
}






