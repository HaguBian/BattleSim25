// Map.cpp
#include "Map.h"

Map::Map(float width, float height)
{
	border.setSize({ width, height });
	border.setFillColor(sf::Color::Transparent);
	border.setOutlineThickness(3.f);
	border.setOutlineColor(sf::Color::Magenta);
	border.setPosition(0.f, 0.f);
}

void Map::draw(sf::RenderWindow& window)
{
	window.draw(border);
}

bool Map::isInside(const sf::Vector2f& pos) const
{
	return pos.x >= 0 && pos.x <= border.getSize().x
		&& pos.y >= 0 && pos.y <= border.getSize().y;
}

sf::FloatRect Map::getBounds() const
{
	return border.getGlobalBounds();
}
