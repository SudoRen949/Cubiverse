#pragma once

#ifndef _VOX2_TEXTURE_H_
#define _VOX2_TEXTURE_H_

#include <GL/gl.h>
#include <cstring>
#include <SFML/Graphics.hpp>

class Texture
{
	public:

		Texture();
		~Texture();
		void load(std::string file);
		void bind();
		void unBind(); // will need it in case.
		void calculateHMap(int block);
		float getMap(int block, int location);
		unsigned int getTextureData();
		const unsigned char *getImageData();
		sf::Image getImage();
		sf::Texture getTexture();
		sf::Vector2f getSection();
		sf::Vector2i getNBlocks();

	private:

		float *texLocation;
		unsigned int texData, skip;
		sf::Image imgClass;
		sf::Texture texClass;
		sf::Vector2f section;
};

#endif /* _VOX2_TEXTURE_H_ */
