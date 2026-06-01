
#include "window.h"

Window::Window()
{
	windowContext.depthBits = 24;
	windowSize = sf::Vector2i(840,480);
	window.create(sf::VideoMode(windowSize.x,windowSize.y),"Cubiverse 1.0a",sf::Style::Default,windowContext);

	world = new World();
}

Window::~Window()
{
	delete world;

	this->close();
}

void Window::run()
{
	float delta = 0.0f;

	sf::Clock clock;

	camera.setPosition(sf::Vector3f(world->getMap().getSize().x/2, world->getNoiseHeight() + 256, world->getMap().getSize().z/2));

	while (window.isOpen())
	{
		window.setFramerateLimit(0);
		window.setView(sf::View(sf::FloatRect(0,0,window.getSize().x,window.getSize().y)));

		delta = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		delta /= 128;

		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
				case sf::Event::Closed:
					this->close();
					break;
				case sf::Event::Resized:
					glViewport(0,0,window.getSize().x,window.getSize().y);
					break;
				default:
					break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

		window.clear();

		/* game scene */

		camera.setRenderDistance(world->getRenderDistance());
		camera.run(world,&window,delta);

		world->setViewProperty(camera.getViewProperty());
		world->render(camera.getPosition(),camera.getRaycaster());

		camera.draw(world,&window);

		window.display();
	}
}

void Window::close()
{
	window.close();

	std::cout << "Window closed!" << std::endl;

	exit(EXIT_SUCCESS);
}
