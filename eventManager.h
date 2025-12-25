#pragma once
#include <SFML/Graphics.hpp>
const float CAMERA_SPEED = 1000.f;

void ProcessEvent(sf::RenderWindow &window, sf::Vector2f move)
{
    sf::Event event;

    while (window.pollEvent(event))
    {
		// Window controls
        if (event.type == sf::Event::Closed)
            window.close();
    }
}