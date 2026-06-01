#include "clouds.h"

Clouds::Clouds()
{
    yheight = moveX = 0.0f;
    updateDraw = true;
    shapeID = 0;
	cloudSize = 10000.0f;
	texture.load("assets/textures/sky/clouds.png");
}

Clouds::~Clouds()
{
	glDeleteLists(shapeID,1);
}

void Clouds::render(sf::Vector3i position)
{
	if (updateDraw)
	{
		shapeID = (UInt)glGenLists(1);

		glNewList((GLuint)shapeID,GL_COMPILE);

			glColor3f(1,1,1);

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);

			glDisable(GL_CULL_FACE);

			glAlphaFunc(GL_GEQUAL,0.5f);

			texture.bind();

			glBegin(GL_QUADS);
				glTexCoord2f(0,0); glVertex3f(        0,1,	      0);
				glTexCoord2f(1,0); glVertex3f(cloudSize,1,		  0);
				glTexCoord2f(1,1); glVertex3f(cloudSize,1,cloudSize);
				glTexCoord2f(0,1); glVertex3f(		  0,1,cloudSize);
			glEnd();

		glEndList();

		updateDraw = false;
	}

	if (shapeID > 0)
	{
		if (moveX >= 1000) moveX = 0;

		yheight = 195 * BLOCK_SIZE;

		moveX += 0.3;

		for (int x = (position.x / cloudSize) - 2; x <= (position.x / cloudSize) + 1; x++)
		for (int z = (position.z / cloudSize) - 2; z <= (position.z / cloudSize) + 1; z++)
		{
			glPushMatrix();

			glTranslatef((x*cloudSize) + moveX, yheight, z*cloudSize);

			glCallList(shapeID);

			glPopMatrix();
		}
	}
}

