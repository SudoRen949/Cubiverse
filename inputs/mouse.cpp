
#include "mouse.h"

int mouseTimer = 0;

bool Mouse::isPressed(sf::Mouse::Button button)
{
	return (sf::Mouse::isButtonPressed(button)) ? true : false;
}

bool Mouse::isClicked(sf::Mouse::Button button)
{
	if (mouseTimer <= 10) mouseTimer++;

	if (isPressed(button) && mouseTimer >= 10)
	{
		mouseTimer = 0;
		return true;
	}
	return false;
}

bool Mouse::isReleased(sf::Mouse::Button button)
{
	return (sf::Mouse::isButtonPressed(button)) ? false : true;
}
