
#include "text.h"


Text::Text()
{
	myFont.loadFromFile("assets/default-font.ttf");
	textgfx.setFont(myFont);
	textgfx.setOutlineColor(sf::Color::Black);
	textgfx.setOutlineThickness(1.0f);
}

Text::Text(std::string msg, sf::Vector2f pos)
{
	::Text();
	setText(msg);
	setPosition(pos);
}

void Text::setText(std::string msg)
{
	message = msg.c_str();
	textgfx.setString(sf::String(msg));
}

void Text::setPosition(sf::Vector2f pos)
{
	position = pos;
}

void Text::setCharSize(UInt sz)
{
	csz = sz;
	textgfx.setCharacterSize(sz);
}

void Text::setAlignment(std::string alignment)
{
	if (alignment == "center")
		textgfx.setOrigin((textgfx.getLocalBounds().left+textgfx.getLocalBounds().width)/2,
		(textgfx.getLocalBounds().left+textgfx.getLocalBounds().height)/2);
	else if (alignment == "left")
		textgfx.setOrigin(0.0f,0.0f);
	else if (alignment == "right")
		textgfx.setOrigin(textgfx.getLocalBounds().left+textgfx.getLocalBounds().width,0.0f);
}

void Text::draw(sf::RenderWindow *window)
{
	textgfx.setPosition(position);
	window->draw(textgfx);
}

std::string Text::getStr()
{
	return message;
}

UInt Text::getCharSize()
{
	return csz;
}

