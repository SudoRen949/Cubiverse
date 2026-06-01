
#include "skybox.h"

Skybox::Skybox()
{
	boxSize = 550;
	updateDraw = true;
	shapeID = 0;
	texture[0].load("assets/textures/sky/sky_top.jpg");
	texture[1].load("assets/textures/sky/sky_side.jpg");
	texture[2].load("assets/textures/sky/sky_bottom.jpg");
}

Skybox::~Skybox()
{
	glDeleteLists(shapeID,1);
}

void Skybox::render(sf::Vector3i position)
{
	if (updateDraw) {

		shapeID = glGenLists(1);

		glNewList(shapeID,GL_COMPILE);

		glEnable(GL_TEXTURE_2D);

		glColor3f(1.f,1.f,1.f);

		texture[0].bind();

		glBegin(GL_QUADS);

		// TOP FACE
		glTexCoord2f(0,0); glVertex3f(-boxSize*10,boxSize,-boxSize*10);
		glTexCoord2f(1,0); glVertex3f( boxSize*10,boxSize,-boxSize*10);
		glTexCoord2f(1,1); glVertex3f( boxSize*10,boxSize, boxSize*10);
		glTexCoord2f(0,1); glVertex3f(-boxSize*10,boxSize, boxSize*10);

		glEnd();

		texture[2].bind();

		glBegin(GL_QUADS); // CREATE LARGE CUBE

		// BOTTOM FACE
		glTexCoord2f(0,1); glVertex3f(-boxSize*10,-boxSize, boxSize*10);
		glTexCoord2f(1,1); glVertex3f( boxSize*10,-boxSize, boxSize*10);
		glTexCoord2f(1,0); glVertex3f( boxSize*10,-boxSize,-boxSize*10);
		glTexCoord2f(0,0); glVertex3f(-boxSize*10,-boxSize,-boxSize*10);

		glEnd();

		texture[1].bind();

		glBegin(GL_QUADS); // CREATE LARGE CUBE

		// LEFT FACE
		glTexCoord2f(1,0); glVertex3f(-boxSize, boxSize, boxSize);
		glTexCoord2f(0,1); glVertex3f(-boxSize,-boxSize, boxSize);
		glTexCoord2f(1,1); glVertex3f(-boxSize,-boxSize,-boxSize);
		glTexCoord2f(0,0); glVertex3f(-boxSize, boxSize,-boxSize);
		// RIGHT
		glTexCoord2f(1,0); glVertex3f(boxSize, boxSize,-boxSize);
		glTexCoord2f(0,1); glVertex3f(boxSize,-boxSize,-boxSize);
		glTexCoord2f(1,1); glVertex3f(boxSize,-boxSize, boxSize);
		glTexCoord2f(0,0); glVertex3f(boxSize, boxSize, boxSize);
		// FRONT
		glTexCoord2f(1,1); glVertex3f( boxSize,-boxSize,boxSize);
		glTexCoord2f(0,1); glVertex3f(-boxSize,-boxSize,boxSize);
		glTexCoord2f(1,0); glVertex3f(-boxSize, boxSize,boxSize);
		glTexCoord2f(0,0); glVertex3f( boxSize, boxSize,boxSize);
		// BACK
		glTexCoord2f(1,1); glVertex3f(-boxSize,-boxSize,-boxSize);
		glTexCoord2f(0,1); glVertex3f( boxSize,-boxSize,-boxSize);
		glTexCoord2f(1,0); glVertex3f( boxSize, boxSize,-boxSize);
		glTexCoord2f(0,0); glVertex3f(-boxSize, boxSize,-boxSize);

		glEnd();

		glEndList();

		updateDraw = false;

	}

	if (shapeID <= 0) return;

	glPushMatrix();

		glTranslatef(position.x,position.y,position.z);

		glCallList(shapeID);

	glPopMatrix();

}

void Skybox::setSize(int size)
{
	if (size <= 0) return;
	boxSize = size;
}

int Skybox::getSize()
{
	return boxSize;
}

int Skybox::getDefaultSize()
{
	return boxSize;
}
