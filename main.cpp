#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

#include "EventManager.h"
#include "Camera2D.h"
#include "Map.h"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGH = 720;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGH), "BattleSim25");
	window.setFramerateLimit(60);

	Map map(2000.f, 2000.f);
	Camera2D camera((float)WINDOW_WIDTH, (float)WINDOW_HEIGH);

	sf::Clock clock;

	while (window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			// Window handling
			if (event.type == sf::Event::Closed)
				window.close();
			
			// Camera handling
			camera.handleEvent(event, window);			
		}
		camera.update(deltaTime, window);
	
		window.clear(sf::Color::Black);
		camera.apply(window);
		map.draw(window);
		
		window.display();
	}

	return 0;
}
