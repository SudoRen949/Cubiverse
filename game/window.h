#pragma once

#ifndef _VOX2_WINDOW_H_
#define _VOX2_WINDOW_H_

#include <SFML/Graphics.hpp>

#include "camera.h"
#include "world.h"

class Window
{
	public:

		Window();
		~Window();
		void run();
		void close();

	private:

		Camera camera;
		sf::Vector2i windowSize;
		sf::ContextSettings windowContext;
		sf::RenderWindow window;
		sf::Event windowEvent;
		World* world;
};

#endif /* _VOX2_WINDOW_H_ */
