#pragma once

#ifndef _VOX2_SKYBOX_H_
#define _VOX2_SKYBOX_H_

#include <SFML/Graphics.hpp>

#include "../maths/texture.h"
#include "chunk.h"
#include "../defines.h"

class Skybox
{
	public:

		Skybox();
		~Skybox();
		void render(sf::Vector3i position);
		void setSize(int size);
		int getSize();
		int getDefaultSize();

	private:

		int boxSize;
		bool updateDraw;
		UInt shapeID;
		Texture texture[3];
};

#endif /* _VOX2_SKYBOX_H_ */
