#pragma once

#ifndef VOX2_KEYBOARD_H
#define VOX2_KEYBOARD_H

#include <SFML/Window/Keyboard.hpp>

class Keyboard
{
	public:

		static bool isKeyPressed(sf::Keyboard::Key key);
		static bool isKeyTyped(sf::Keyboard::Key key);
		static bool isKeyReleased(sf::Keyboard::Key key);

};

#endif /* VOX2_KEYBOARD_H */
