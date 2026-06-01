
/*
	Code from GameMaker Minecraft by Florevka.
*/

#include "raycast.h"

Raycast::Raycast()
{
	angle 	= 0.0f;
	aspect 	= 0.0f;
	near 	= 0.0f;
	far 	= 0.0f;
	from 	= sf::Vector3f(0,0,0);
	to 		= sf::Vector3f(0,0,0);
	up 		= sf::Vector3f(0,0,0);
	reachRange = 5 * BLOCK_SIZE;
	updateDrawing = true;
	staticShapeID = 0;
}

void Raycast::setCamera(sf::Vector3f from, sf::Vector3f to, sf::Vector3f up)
{
	this->from = from;
	this->to = to - this->from;
	this->up = up;
}

void Raycast::setPerspective(float angle, float aspect, float near, float far)
{
	this->angle = angle;
	this->aspect = aspect;
	this->near = near;
	this->far = far;
}

void Raycast::setReachRange(int range)
{
	reachRange = range;
}

void Raycast::calculateView()
{
	float distance = sqrt( Math::sqr(this->to.x) + Math::sqr(this->to.y) + Math::sqr(this->to.z) );

	this->to /= distance;

	distance = (this->up.x * this->to.x) + (this->up.y * this->to.y) + (this->up.z * this->to.z);

	this->up.x -= distance * this->to.x;
	this->up.y -= distance * this->to.y;
	this->up.z -= distance * this->to.z;

	distance = sqrt( Math::sqr(this->up.x) + Math::sqr(this->up.y) + Math::sqr(this->up.z) );

	this->up /= distance;

	cross.x = (this->up.y * this->to.z) - (this->up.z * this->to.y);
	cross.y = (this->up.z * this->to.x) - (this->up.x * this->to.z);
	cross.z = (this->up.x * this->to.y) - (this->up.y * this->to.x);

	fcullTan.y = tan( Math::degtorad(this->angle) / 2 );
	fcullTan.x = fcullTan.y * this->aspect;
	fcullSec.x = sqrt( 1 + Math::sqr(fcullTan.x) );
	fcullSec.y = sqrt( 1 + Math::sqr(fcullTan.y) );
}

bool Raycast::isHit(Grid grid)
{
	for (int i = 0; i < reachRange; i++)
	{
		int x = floor( (from.x + to.x * i) / BLOCK_SIZE );
		int y = floor( (from.y + to.y * i) / BLOCK_SIZE );
		int z = floor( (from.z + to.z * i) / BLOCK_SIZE );

		if (y < 0) y = 0;

		UShort block = grid.getValueAt(sf::Vector3i(x,y,z));

		if ( x >= 0 && x <= grid.getSize().x && z >= 0 && z <= grid.getSize().z)
		{
			if ( block != Types::AIR && !Types::checkLiquids(block) && y >= 0 && y <= grid.getSize().y )
			{
				positionTo.x = floor( (from.x + to.x * i) / BLOCK_SIZE );
				positionTo.y = floor( (from.y + to.y * i) / BLOCK_SIZE );
				positionTo.z = floor( (from.z + to.z * i) / BLOCK_SIZE );

				i -= 1;

				positionFrom.x = floor( (from.x + to.x * i) / BLOCK_SIZE );
				positionFrom.y = floor( (from.y + to.y * i) / BLOCK_SIZE );
				positionFrom.z = floor( (from.z + to.z * i) / BLOCK_SIZE );

				return true;
			}
		}
	}
	return false;
}

void Raycast::drawSelector()
{
	if (updateDrawing)
	{
		float offset = 0.005f;

		staticShapeID = glGenLists(1);

		glNewList(staticShapeID,GL_COMPILE);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);

		glLineWidth(3.0f);

		glColor3f(0.65f,0.65f,0.65f);

		glBegin(GL_LINE_LOOP);

		glVertex3f(			  -offset,BLOCK_SIZE+offset,           -offset); //
		glVertex3f( BLOCK_SIZE+offset,BLOCK_SIZE+offset,           -offset);
		glVertex3f( BLOCK_SIZE+offset,BLOCK_SIZE+offset, BLOCK_SIZE+offset);
		glVertex3f(           -offset,BLOCK_SIZE+offset, BLOCK_SIZE+offset);

		glEnd();

		glBegin(GL_LINE_LOOP);

		glVertex3f(           -offset,-offset, BLOCK_SIZE+offset); //
		glVertex3f( BLOCK_SIZE+offset,-offset, BLOCK_SIZE+offset);
		glVertex3f( BLOCK_SIZE+offset,-offset,           -offset);
		glVertex3f(           -offset,-offset,           -offset);

		glEnd();

		glBegin(GL_LINE_LOOP);

		glVertex3f(-offset,           -offset, BLOCK_SIZE+offset); //
		glVertex3f(-offset,           -offset,           -offset);
		glVertex3f(-offset, BLOCK_SIZE+offset,           -offset);
		glVertex3f(-offset, BLOCK_SIZE+offset, BLOCK_SIZE+offset);

		glEnd();

		glBegin(GL_LINE_LOOP);

		glVertex3f( BLOCK_SIZE+offset,           -offset,           -offset); //
		glVertex3f( BLOCK_SIZE+offset, BLOCK_SIZE+offset,           -offset);
		glVertex3f( BLOCK_SIZE+offset, BLOCK_SIZE+offset, BLOCK_SIZE+offset);
		glVertex3f( BLOCK_SIZE+offset,           -offset, BLOCK_SIZE+offset);

		glEnd();

		glBegin(GL_LINE_LOOP);

		glVertex3f( BLOCK_SIZE+offset,           -offset, BLOCK_SIZE+offset); //
		glVertex3f(           -offset,           -offset, BLOCK_SIZE+offset);
		glVertex3f(           -offset, BLOCK_SIZE+offset, BLOCK_SIZE+offset);
		glVertex3f( BLOCK_SIZE+offset, BLOCK_SIZE+offset, BLOCK_SIZE+offset);

		glEnd();

		glBegin(GL_LINE_LOOP);

		glVertex3f(           -offset,           -offset,-offset); //
		glVertex3f( BLOCK_SIZE+offset,           -offset,-offset);
		glVertex3f( BLOCK_SIZE+offset, BLOCK_SIZE+offset,-offset);
		glVertex3f(           -offset, BLOCK_SIZE+offset,-offset);

		glEnd();

		glEnable(GL_TEXTURE_2D);

		glLineWidth(1.0f);

		glEndList();

		updateDrawing = false;

	}

	glPushMatrix();

		glTranslatef(floor(positionTo.x)*BLOCK_SIZE,floor(positionTo.y)*BLOCK_SIZE,floor(positionTo.z)*BLOCK_SIZE);

		glCallList(staticShapeID);

	glPopMatrix();
}

bool Raycast::surfaceHidden(sf::Vector3f position, float radius)
{
	float x1 = position.x - from.x;
	float y1 = position.y - from.y;
	float z1 = position.z - from.z;

	float z2 = to.x * x1 + to.y * y1 + to.z * z1;

	if ( z2 < near - radius || z2 > (far/2) + radius) return false;

	float x2 = cross.x * x1 + cross.y * y1 + cross.z * z1;

	if ( abs(x2) > z2 * fcullTan.x + radius * fcullSec.x ) return false;

	float y2 = up.x * x1 + up.y * y1 + up.z * z1;

	if ( abs(y2) > z2 * fcullTan.y + radius * fcullSec.y ) return false;

	return true;
}

sf::Vector3f Raycast::getRayAt()
{
	return positionTo;
}

sf::Vector3f Raycast::getRayFrom()
{
	return positionFrom;
}
