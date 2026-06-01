
#include "camera.h"

Camera::Camera()
{
	mouseSensitivity = 6.0f;
	fov = 70.0f;
	near = 1.0f;
	far = 0.0f;

	dimension = sf::Vector3f(0.375f,0.8125f,0.375);
	dimension *= static_cast<float>(BLOCK_SIZE);

	speed = SPEED_NORMAL;
	friction = 0.75f;
	gravity = 15;
	camDistance = -1;
	cameraType = 1; // 1 = normal, 2 = back, 3 = front

	texture.loadFromFile("assets/pointer.png");

	cursor.setTexture(&texture);
	cursor.setSize(sf::Vector2f(texture.getSize().x,texture.getSize().y));
	cursor.setOrigin(cursor.getSize().x/2,cursor.getSize().y/2);

	/* vision texture */

	size_t visionArraySize = sizeof(vision) / sizeof(sf::RectangleShape);

	visionTexture[0].loadFromFile("assets/textures/vision/water.png");
	visionTexture[1].loadFromFile("assets/textures/vision/lava.png");

	for (size_t i = 0; i < visionArraySize; ++i)
	{
		vision[i].setTexture(&visionTexture[i]);
		vision[i].setPosition(0,0);
	}

	/* messages */

	for (auto& msg : message)
	{
		msg.setCharSize(18);
		msg.setText(" ");
	}

	for (auto& dmsg : debugMessage)
	{
		dmsg.setCharSize(18);
		dmsg.setText(" ");
	}

	viewDebug = true;

	/* water fog */

	fogColor[0][0] = 0.0f;
	fogColor[0][1] = 0.0f;
	fogColor[0][2] = 1.0f;
	fogColor[0][3] = 1.0f;

	/* lava fog */

	fogColor[1][0] = 1.0f;
	fogColor[1][1] = 0.6f;
	fogColor[1][2] = 0.0f;
	fogColor[1][3] = 1.0f;
}

void Camera::run(World *world, sf::RenderWindow *window, float dtime)
{
	rayIsHit = false;

	/* -- Messages -- */

	for (size_t i = 0; i < sizeof(message) / sizeof(Text); ++i)	{
		if (message[i].getStr().length() > 0) {
			message[i].setText(" ");
		}
	}

	if ((position.y/BLOCK_SIZE) >= WORLD_SIZE_V) message[0].setText("You've reached the world height!");

	setupDebugMessages(world,window,dtime);

	/* -- Keys -- */

	if (window->hasFocus())
	{
		if (Keyboard::isKeyTyped(sf::Keyboard::F5))
		{
			if (cameraType > 3) cameraType = 1;
			cameraType++;
		}

		if (Keyboard::isKeyTyped(sf::Keyboard::F3)) viewDebug = !viewDebug;

		if (Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (onGround && !onWater && !onLava && !underWater && !underLava)  // JUMP
			{
				direction.y = DEFAULT_JUMP_HEIGHT;
			}
			else if (!onGround) // SWIM
			{
				if (underWater || onWater)
				{
					float X = (position.x / BLOCK_SIZE);
					float Y = (position.y / BLOCK_SIZE);
					float Z = (position.z / BLOCK_SIZE);

					if ((Types::checkSolids(world->getMap().getValueAt(sf::Vector3i(floor(X-0.40),Y,Z))) ||
						 Types::checkSolids(world->getMap().getValueAt(sf::Vector3i(floor(X+0.40),Y,Z))) ||
						 Types::checkSolids(world->getMap().getValueAt(sf::Vector3i(X,Y,floor(Z-0.40)))) ||
						 Types::checkSolids(world->getMap().getValueAt(sf::Vector3i(X,Y,floor(Z+0.40))))) &&
						 world->getMap().getValueAt(sf::Vector3i(X,Y+1,Z)) == Types::AIR)
					{
						direction.y = DEFAULT_JUMP_HEIGHT - 2;
					}
					else
					{
						direction.y += DEFAULT_WATER_SWIM_SPEED;
					}
				}
				else if (underLava || onLava)
				{
					float X = (position.x / BLOCK_SIZE);
					float Y = (position.y / BLOCK_SIZE);
					float Z = (position.z / BLOCK_SIZE);

					if ((Types::checkSolids(world->getMap().getValueAt(sf::Vector3i(floor(X-0.40),Y,Z))) ||
						 Types::checkSolids(world->getMap().getValueAt(sf::Vector3i(floor(X+0.40),Y,Z))) ||
						 Types::checkSolids(world->getMap().getValueAt(sf::Vector3i(X,Y,floor(Z-0.40)))) ||
						 Types::checkSolids(world->getMap().getValueAt(sf::Vector3i(X,Y,floor(Z+0.40))))) &&
						 world->getMap().getValueAt(sf::Vector3i(X,Y+1,Z)) == Types::AIR)
					{
						direction.y = DEFAULT_JUMP_HEIGHT - 2;
					}
					else
					{
						direction.y += DEFAULT_LAVA_SWIM_SPEED;
					}
				}
			}
		}

		if (Keyboard::isKeyPressed(sf::Keyboard::LShift)) // sunk down
		{
			if (onGround)
			{
				if (!onWater && !onLava && !underWater && !underLava) speed = SPEED_NORMAL - 5.00f;
				else if (onWater || underWater) speed = SPEED_ONWATER - 1.00f;
				else if (onLava || underLava) speed = SPEED_ONLAVA - 1.00f;
			}
			else if (onWater || underWater) direction.y -= (DEFAULT_WATER_SWIM_SPEED + 0.15f);
			else if (onLava || underLava) direction.y -= (DEFAULT_LAVA_SWIM_SPEED + 0.25f);
		}

		if (Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			direction.x = -sin(Math::degtorad(mouseAngle.x)) * speed;
			direction.z = -cos(Math::degtorad(mouseAngle.x)) * speed;
		}

		if (Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			direction.x =  sin(Math::degtorad(mouseAngle.x)) * speed;
			direction.z =  cos(Math::degtorad(mouseAngle.x)) * speed;
		}

		if (Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			direction.x =  sin(Math::degtorad(mouseAngle.x+90)) * speed;
			direction.z =  cos(Math::degtorad(mouseAngle.x+90)) * speed;
		}

		if (Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			direction.x =  sin(Math::degtorad(mouseAngle.x-90)) * speed;
			direction.z =  cos(Math::degtorad(mouseAngle.x-90)) * speed;
		}
	}

	/* -- Physics -- */

	inLiquids(world->getMap(),position,dtime);

	position.x += direction.x * dtime;

	collide(world->getMap(),sf::Vector3f(direction.x,0.0f,0.0f));

	position.y += direction.y * dtime;

	collide(world->getMap(),sf::Vector3f(0.0f,direction.y,0.0f));

	position.z += direction.z * dtime;

	collide(world->getMap(),sf::Vector3f(0.0f,0.0f,direction.z));

	direction.x *= friction;
	direction.z *= friction;

	/* -- Camera View -- */

	if (window->hasFocus())
	{
		window->setMouseCursorVisible(false);
		window->setMouseCursorGrabbed(true);

		mousePosition.x = window->getSize().x / 2;
		mousePosition.y = window->getSize().y / 2;

		mouseAngle.x += static_cast<float>(mousePosition.x - sf::Mouse::getPosition(*window).x) / mouseSensitivity;
		mouseAngle.y += static_cast<float>(mousePosition.y - sf::Mouse::getPosition(*window).y) / mouseSensitivity;
		mouseAngle.y = Math::clamp(mouseAngle.y,-89,89);

		if (mouseAngle.x < 0) mouseAngle.x = 360;
		if (mouseAngle.x > 360) mouseAngle.x = 0;

		sf::Mouse::setPosition(mousePosition,*window);
	}

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);

	glDepthRange(0.5f,1.0f);

	glFogi(GL_FOG_MODE,GL_LINEAR);
	glFogf(GL_FOG_DENSITY,0.001f);

	glHint(GL_FOG_HINT,GL_NICEST);

	viewPosition = sf::Vector3f(position.x, position.y + (dimension.y - 2.4), position.z);

	viewEye = sf::Vector3f(
			viewPosition.x - sin(Math::degtorad(mouseAngle.x)),
			viewPosition.y + tan(Math::degtorad(mouseAngle.y)),
			viewPosition.z - cos(Math::degtorad(mouseAngle.x)));

	viewUp = sf::Vector3f(0,1,0);

	far = 1000.0f * (world->getRenderDistance() + 4);

	aspect = (float) window->getSize().x / (float) window->getSize().y;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fov,aspect,near,far);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	switch (cameraType)
	{
		case 1:
				camDistance = 1;
			break;
		case 2:
				camDistance = -50;
			break;
		case 3:
				camDistance = 50;
			break;
		default: break;
	}

	if (cameraType == 1 || cameraType == 2) glTranslatef(0,0,static_cast<GLfloat>(camDistance));
	else if (cameraType == 3)
	{
		glRotatef(180,0,1,0);
		glTranslatef(0,0,static_cast<GLfloat>(camDistance));
	}

	gluLookAt(viewPosition.x,viewPosition.y,viewPosition.z,viewEye.x,viewEye.y,viewEye.z,viewUp.x,viewUp.y,viewUp.z);

	/* Raycast */

	raycast.setCamera(viewPosition,viewEye,viewUp);
	raycast.setPerspective(fov,aspect,near,far);
	raycast.calculateView();

	if (raycast.isHit(world->getMap()) && window->hasFocus())
	{
		rayIsHit = true;

		float xx,yy,zz, rx,ry,rz;

		// player position (current)
		xx = position.x / BLOCK_SIZE;
		zz = position.z / BLOCK_SIZE;
		yy = position.y / BLOCK_SIZE;

		// raycast position (previous)
		rx = raycast.getRayFrom().x;
		rz = raycast.getRayFrom().z;
		ry = raycast.getRayFrom().y;

		if (buildOn(sf::Vector3f(xx,yy,zz),sf::Vector3f(rx,ry,rz)))
		{
			if (Mouse::isClicked(sf::Mouse::Right))
				world->getChunk()->addBlock(world->getMap(),sf::Vector3i(raycast.getRayFrom()),Types::COBBLE);
		}

		if (Mouse::isClicked(sf::Mouse::Left)) world->getChunk()->removeBlock(world->getMap(),sf::Vector3i(raycast.getRayAt()));
	}
}

void Camera::draw(World *world, sf::RenderWindow *window)
{
	/* 3D Drawing */

	if (rayIsHit) raycast.drawSelector();

	if (underWater)
	{
		glFogfv(GL_FOG_COLOR,fogColor[0]);
		glFogf(GL_FOG_START,200.0f);
		glFogf(GL_FOG_END,300.0f);
		glEnable(GL_FOG);
	}
	else if (underLava)
	{
		glFogfv(GL_FOG_COLOR,fogColor[1]);
		glFogf(GL_FOG_START,200.0f);
		glFogf(GL_FOG_END,300.0f);
		glEnable(GL_FOG);
	}
	else glDisable(GL_FOG);

	/* 2D Drawing */

	window->pushGLStates();

	cursor.setPosition(mousePosition.x,mousePosition.y);

	vision[0].setSize(sf::Vector2f(window->getSize().x,window->getSize().y));
	vision[1].setSize(sf::Vector2f(window->getSize().x,window->getSize().y));

	if (underWater) window->draw(vision[0]);
	if (underLava) window->draw(vision[1]);

	window->draw(cursor);

	message[0].setAlignment("center");
	message[0].setPosition(sf::Vector2f(window->getSize().x/2,window->getSize().y-(message[0].getCharSize()+5)));
	message[0].draw(window);

	drawDebugMessages(window);

	window->popGLStates();
}

void Camera::collide(Grid grid, sf::Vector3f cdirection)
{
	/* check if on boundary */

	if (position.y <= -256 * BLOCK_SIZE) { position.y = -256 * BLOCK_SIZE; direction.y = 0; }
	if (position.x - dimension.x <= 0) position.x = dimension.x;
	if (position.z - dimension.z <= 0) position.z = dimension.z;
	if (position.x + dimension.x > grid.getSize().x * BLOCK_SIZE) position.x = (grid.getSize().x * BLOCK_SIZE) - dimension.x;
	if (position.z + dimension.z > grid.getSize().z * BLOCK_SIZE) position.z = (grid.getSize().z * BLOCK_SIZE) - dimension.z;

	/* check collision on each side of the block */

	Y1 = (position.y - dimension.y) / BLOCK_SIZE;
	X1 = (position.x - dimension.x) / BLOCK_SIZE;
	Z1 = (position.z - dimension.z) / BLOCK_SIZE;

	Y2 = (position.y + dimension.y) / BLOCK_SIZE;
	X2 = (position.x + dimension.x) / BLOCK_SIZE;
	Z2 = (position.z + dimension.z) / BLOCK_SIZE;

	for (int y = Y1; y < Y2; ++y)
	for (int x = X1; x < X2; ++x)
	for (int z = Z1; z < Z2; ++z)
	{
		if (Types::checkSolids(grid.getValueAt(sf::Vector3i(x,y,z))))
		{
			if (cdirection.y < 0) { position.y = y * BLOCK_SIZE + BLOCK_SIZE + dimension.y; 	direction.y = 0; onGround = true; 	}
			if (cdirection.y > 0) { position.y = y * BLOCK_SIZE - dimension.y; 					direction.y = 0; onGround = false; 	}
			if (cdirection.x < 0) { position.x = x * BLOCK_SIZE + BLOCK_SIZE + dimension.x; 	direction.x = 0; }
			if (cdirection.x > 0) { position.x = x * BLOCK_SIZE - dimension.x; 					direction.x = 0; }
			if (cdirection.z < 0) { position.z = z * BLOCK_SIZE + BLOCK_SIZE + dimension.z; 	direction.z = 0; }
			if (cdirection.z > 0) { position.z = z * BLOCK_SIZE - dimension.z; 					direction.z = 0; }
		}
	}
}

void Camera::setPosition(sf::Vector3f position)
{
	this->position = sf::Vector3f(position.x * BLOCK_SIZE, position.y * BLOCK_SIZE, position.z * BLOCK_SIZE);
}

void Camera::setRenderDistance(int distance)
{
	renderDistance = distance;
}

Raycast Camera::getRaycaster()
{
	return raycast;
}

sf::Vector3f Camera::getPosition()
{
	return position;
}

sf::Vector3f Camera::getDimension()
{
	return dimension;
}

sf::Vector3f Camera::getViewProperty()
{
	return sf::Vector3f(fov,near,far);
}


/* PRIVATE FUNCTIONS */


void Camera::inLiquids(Grid grid, sf::Vector3f pos, float dtime)
{
	float X = (pos.x / BLOCK_SIZE);
	float Y = (pos.y / BLOCK_SIZE);
	float Z = (pos.z / BLOCK_SIZE);

	onLava = onWater = false;
	underLava = underWater = false;

	if 		(grid.getValueAt(sf::Vector3i(floor(X+0.15),floor(Y+0.70),Z)) == Types::WATER ||
			 grid.getValueAt(sf::Vector3i(floor(X-0.15),floor(Y+0.70),Z)) == Types::WATER ||
			 grid.getValueAt(sf::Vector3i(X,floor(Y+0.70),floor(Z+0.15))) == Types::WATER ||
			 grid.getValueAt(sf::Vector3i(X,floor(Y+0.70),floor(Z-0.15))) == Types::WATER)
	{
		underWater = true;
	}
	else if (grid.getValueAt(sf::Vector3i(floor(X+0.15),floor(Y+0.60),Z)) == Types::LAVA ||
			 grid.getValueAt(sf::Vector3i(floor(X-0.15),floor(Y+0.60),Z)) == Types::LAVA ||
			 grid.getValueAt(sf::Vector3i(X,floor(Y+0.60),floor(Z+0.15))) == Types::LAVA ||
			 grid.getValueAt(sf::Vector3i(X,floor(Y+0.60),floor(Z-0.15))) == Types::LAVA)
	{
		underLava = true;
	}

	if 		(grid.getValueAt(sf::Vector3i(X,Y,Z)) == Types::WATER) onWater = true;
	else if (grid.getValueAt(sf::Vector3i(X,Y,Z)) == Types::LAVA) onLava = true;

	/* Update Gravity */

	if 		(onWater || underWater) { gravity = 0.85f; direction.y *= 0.95f; }
	else if (onLava || underLava) 	{ gravity = 0.50f; direction.y *= 0.95f; }
	else gravity = 15;

	if (!onGround) direction.y -= gravity * dtime;
	else onGround = false;

	if (onWater || underWater) speed = SPEED_ONWATER;
	else if (onLava || underLava) speed = SPEED_ONLAVA;
	else speed = SPEED_NORMAL;
}

bool Camera::buildOn(sf::Vector3f pos1, sf::Vector3f pos2)
{
	sf::Vector3f s = sf::Vector3f((dimension.x/BLOCK_SIZE)+0.9985f,
	(dimension.y/BLOCK_SIZE)+1.00f,(dimension.z/BLOCK_SIZE)+0.9985f);

	// prevents player to build inside it's body...

	if (pos2.x <= floor(pos1.x - s.x) || pos2.x >= floor(pos1.x + s.x) ||
		pos2.z <= floor(pos1.z - s.z) || pos2.z >= floor(pos1.z + s.z) ||
		pos2.y <= floor(pos1.y - s.y) || pos2.y >= floor(pos1.y + s.y)) return true;

	return false;
}

void Camera::setupDebugMessages(World *world, sf::RenderWindow *window, float dtime)
{
	if (viewDebug)
	{
		for (auto& dms : debugMessage) dms.setText(" ");

		/* chunk update message */

		debugMessage[0].setText("Chunks Updates: " + std::to_string(world->getChunk()->getUpdateCount()));

		/* direction message */

		int fdirX = static_cast<int>(mouseAngle.x);
		std::string facing = "Facing: ";
		if (fdirX >= 315 || fdirX <= 40) facing += "North -Z";
		else if (fdirX >= 250 && fdirX <= 290) facing += "East +X";
		else if (fdirX >= 150 && fdirX <= 210) facing += "South +Z";
		else if (fdirX >= 70 && fdirX <= 110) facing += "West -X";
		debugMessage[1].setText(facing);

		/* player position */

		debugMessage[2].setText("Position (XYZ): " + std::to_string(position.x / BLOCK_SIZE) + ", " + std::to_string(position.y / BLOCK_SIZE) + ", " + std::to_string(position.z / BLOCK_SIZE));

		/* tick speed */

		debugMessage[3].setText("Tick Speed: " + std::to_string(DEFAULT_TICK_SPEED));

		/* game name */

		debugMessage[4].setText("Cubiverse alpha-1.0");

		/* seed */

		debugMessage[5].setText("Seed: " + std::to_string(world->getSeed()));
	}
}

void Camera::drawDebugMessages(sf::RenderWindow *window)
{
	if (viewDebug)
	{
		for (auto& dms : debugMessage) dms.draw(window);

		debugMessage[5].setAlignment("right");

		debugMessage[0].setPosition(sf::Vector2f(debugMessage[1].getCharSize(), message[1].getCharSize()*2));
		debugMessage[1].setPosition(sf::Vector2f(debugMessage[1].getCharSize(), message[1].getCharSize()*3));
		debugMessage[2].setPosition(sf::Vector2f(debugMessage[1].getCharSize(), message[1].getCharSize()*4));
		debugMessage[3].setPosition(sf::Vector2f(debugMessage[1].getCharSize(), message[1].getCharSize()*5));
		debugMessage[4].setPosition(sf::Vector2f(debugMessage[1].getCharSize(), message[1].getCharSize()*1));
		debugMessage[5].setPosition(sf::Vector2f(window->getSize().x - debugMessage[1].getCharSize(), debugMessage[1].getCharSize()*1));
	}
}




