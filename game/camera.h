#pragma once

#ifndef _VOX2_CAMERA_H_
#define _VOX2_CAMERA_H_

#include <string>
#include <GL/glu.h>
#include <SFML/Graphics.hpp>

#include "../maths/grid.h"
#include "../maths/maths.h"
#include "../defines.h"
#include "../maths/raycast.h"
#include "world.h"
#include "text.h"
#include "../inputs/mouse.h"
#include "../inputs/keyboard.h"

class Camera
{
	public:

		Camera();

		void run(World *world, sf::RenderWindow *window, float dtime);
		void draw(World *world, sf::RenderWindow *window);
		void collide(Grid grid, sf::Vector3f cdirection);
		void setPosition(sf::Vector3f position);
		void setRenderDistance(int distance);

		Raycast getRaycaster();

		sf::Vector3f getPosition();
		sf::Vector3f getDimension();
		sf::Vector3f getViewProperty();

		UInt getCameraType();

	private:

		int camDistance, renderDistance;

		bool onWater, onLava, underWater, underLava;
		bool viewDebug, rayIsHit, onGround;

		float aspect, fogColor[2][4];
		float near, far, speed, gravity;
		float friction, fov, mouseSensitivity;
		float X1,Y1,Z1,X2,Y2,Z2;

		UInt cameraType;

		Raycast raycast;
		Text message[2], debugMessage[8];

		sf::Vector3f position, direction, dimension;
		sf::Vector2f mouseAngle;
		sf::Vector3f viewPosition,viewEye,viewUp;
		sf::Vector2i mousePosition;
		sf::Vector3i blockPosition;
		sf::RectangleShape cursor, vision[2];
		sf::Texture texture, visionTexture[2];

		bool buildOn(sf::Vector3f pos1, sf::Vector3f pos2);
		void inLiquids(Grid grid, sf::Vector3f pos, float dtime);
		void setupDebugMessages(World *world, sf::RenderWindow *window, float dtime);
		void drawDebugMessages(sf::RenderWindow *window);
};

#endif /* _VOX2_CAMERA_H_ */
