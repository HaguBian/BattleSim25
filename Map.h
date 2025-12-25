#pragma once
#include <SFML/Graphics.hpp>

class Map
{
public:
	Map(float width, float height);

	void draw(sf::RenderWindow& window);
	bool isInside(const sf::Vector2f& pos) const;
	sf::FloatRect getBounds() const;

private:
	sf::RectangleShape border;
};