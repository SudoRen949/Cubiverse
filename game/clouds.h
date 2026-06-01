#pragma once

#ifndef _VOX2_CLOUDS_H_
#define _VOX2_CLOUDS_H_

#include <GL/glu.h>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "../maths/texture.h"
#include "../defines.h"
#include "../maths/maths.h"

class Clouds
{
	public:

		Clouds();
		~Clouds();
		void render(sf::Vector3i position);

	private:

		bool updateDraw;
		UInt shapeID;
		float cloudSize, yheight, moveX;
		sf::Vector3i position;
		Texture texture;
};

#endif /* _VOX2_CLOUDS_H_ */
