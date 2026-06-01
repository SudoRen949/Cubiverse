#pragma once

#ifndef VOX2_MOUSE_H
#define VOX2_MOUSE_H

#include <SFML/Window/Mouse.hpp>

class Mouse
{
	public:

		static bool isPressed(sf::Mouse::Button button);
		static bool isClicked(sf::Mouse::Button button);
		static bool isReleased(sf::Mouse::Button button);

};

#endif /* VOX2_MOUSE_H */
