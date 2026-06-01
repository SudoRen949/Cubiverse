#pragma once

#ifndef TEXT_H_
#define TEXT_H_

#include <string>
#include <SFML/Graphics.hpp>

#include "../defines.h"

class Text
{
	public:

		Text();
		Text(std::string msg, sf::Vector2f pos);
		void setText(std::string msg);
		void setPosition(sf::Vector2f pos);
		void setCharSize(UInt sz);
		void setAlignment(std::string alignment);
		void draw(sf::RenderWindow *window);
		std::string getStr();
		UInt getCharSize();

	private:

		std::string message;
		UInt csz;
		sf::Text textgfx;
		sf::Font myFont;
		sf::Vector2f position;
};

#endif // TEXT_H_
