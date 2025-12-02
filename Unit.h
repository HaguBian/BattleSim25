#pragma once
#include <SFML/Graphics.hpp>

class Unit
{
public:
	sf::CircleShape shape;
	int team; // 0 or 1
	int gridX, gridY;

	Unit(int x, int y, int t);

	void Draw(sf::RenderWindow& window, float zoomLevel);
};