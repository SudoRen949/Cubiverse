#include "texture.h"

Texture::Texture()
{
	skip = 0;
	texData = (unsigned int) NULL;
	texLocation = new float[8];
}

Texture::~Texture()
{
	delete[] texLocation;
}

void Texture::load(std::string file)
{
	if (file.length() == 0) return;
	imgClass.loadFromFile(file);
	texClass.loadFromImage(imgClass);
	texData = texClass.getNativeHandle();
	section.x = 16.0f / (float) imgClass.getSize().x;
	section.y = 16.0f / (float) imgClass.getSize().y;
}

void Texture::bind()
{
	/* Uses SFML texture binding technique. */
	//sf::Texture::bind(&texClass);
	glBindTexture(GL_TEXTURE_2D,texData);
}

void Texture::unBind()
{
	/* Uses SFML texture binding technique. */
	glBindTexture(GL_TEXTURE_2D,0);
}

void Texture::calculateHMap(int block)
{
	skip = 0;
	if (block == section.x +  1) skip = 3;
	if (block == section.x + 18) skip = 4;
	if (block == section.x + 51) skip = 5;
	if (block == section.x + 68) skip = 6;
	if (block == section.x + 85) skip = 7;
}

float Texture::getMap(int block, int location)
{
	texLocation[0] = section.x * ((skip + block) - 1);		texLocation[1] = section.y * block;
	texLocation[2] = section.x *       (skip + block);		texLocation[3] = section.y * block;
	texLocation[4] = section.x *       (skip + block);		texLocation[5] = section.y * (block-1);
	texLocation[6] = section.x * ((skip + block) - 1);		texLocation[7] = section.y * (block-1);
	switch (location)
	{
		case 0: return texLocation[0];
		case 1: return texLocation[1];
		case 2: return texLocation[2];
		case 3: return texLocation[3];
		case 4: return texLocation[4];
		case 5: return texLocation[5];
		case 6: return texLocation[6];
		case 7: return texLocation[7];
		default: break;
	}
	return 0.0f;
}

unsigned int Texture::getTextureData()
{
	return texData;
}

const unsigned char* Texture::getImageData()
{
	return imgClass.getPixelsPtr();
}

sf::Image Texture::getImage()
{
	return imgClass;
}

sf::Texture Texture::getTexture()
{
	return texClass;
}

sf::Vector2f Texture::getSection()
{
	return section;
}

sf::Vector2i Texture::getNBlocks()
{
	return sf::Vector2i(texClass.getSize().x/16,texClass.getSize().y/16);
}
