#include "Unit.h"
#include "GridMap.h"

Unit::Unit(int x, int y, int t) : gridX(x), gridY(y), team(t)
{
	shape.setRadius(TILE_SIZE / 2 - 2);
	shape.setOrigin(sf::Vector2f(float(TILE_SIZE / 2 - 2), float(TILE_SIZE / 2 - 2)));
	shape.setFillColor(team == 0 ? sf::Color::Blue : sf::Color::Red);
	shape.setPosition(sf::Vector2f(gridX * TILE_SIZE + TILE_SIZE / 2, gridY * TILE_SIZE + TILE_SIZE / 2));
}

void Unit::Draw(sf::RenderWindow& window, float zoomLevel)
{
    float scaledRadius = (TILE_SIZE / 2.f - 2.f) * zoomLevel;
    shape.setRadius(scaledRadius);
    shape.setOrigin(sf::Vector2f(scaledRadius, scaledRadius));
    shape.setPosition(sf::Vector2f((gridX + 0.5f) * TILE_SIZE * zoomLevel, (gridY + 0.5f) * TILE_SIZE * zoomLevel));

    window.draw(shape);
}
