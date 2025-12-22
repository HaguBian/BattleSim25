#include <SFML/Graphics.hpp>
#include "eventManager.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "BattleSim25");

	while (window.isOpen())
	{
		ProcessEvent(window);

		window.clear(sf::Color::Black);
		window.display();
	}

	return 0;
}
