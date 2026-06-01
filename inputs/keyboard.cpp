
#include "keyboard.h"

int keyTimer = 0;

bool Keyboard::isKeyPressed(sf::Keyboard::Key key)
{
	return (sf::Keyboard::isKeyPressed(key)) ? true : false;
}

bool Keyboard::isKeyReleased(sf::Keyboard::Key key)
{
	return (sf::Keyboard::isKeyPressed(key)) ? false : true;
}

bool Keyboard::isKeyTyped(sf::Keyboard::Key key)
{
	if (keyTimer <= 10) keyTimer++;

	if (isKeyPressed(key) && keyTimer >= 10)
	{
		keyTimer = 0;
		return true;
	}

	return false;
}
