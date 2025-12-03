#include "Unit.h"
#include "GridMap.h"
#include <iostream>


#include "Unit.h"  
#include "GridMap.h"  
#include <iostream>  

Unit::Unit(int x, int y, int f, float def, float shd, float arm, const std::string& logoPath)
	: gridX(x), gridY(y), faction(f), defense(def), shield(shd), armor(arm)
{
	maxHealth = 100.f;  
	curHealth = maxHealth;  

	// Initialize shape  
	float radius = TILE_SIZE / 2.f - 2.f;  
	shape.setRadius(radius);  
	shape.setOrigin(sf::Vector2f(radius, radius));  

	switch (faction)  
	{  
	case 0: shape.setFillColor(sf::Color::Blue); break;  
	case 1: shape.setFillColor(sf::Color::Red); break;  
	case 2: shape.setFillColor(sf::Color::Green); break;  
	case 3: shape.setFillColor(sf::Color::Yellow); break;  
	default: shape.setFillColor(sf::Color::White); break;  
	}  

	shape.setPosition(sf::Vector2f(gridX * TILE_SIZE + TILE_SIZE / 2,  
									gridY * TILE_SIZE + TILE_SIZE / 2));  

	// Load logo texture if provided  
	if (!logoPath.empty())  
	{  
		if (!logoTexture.loadFromFile(logoPath))
		{
			std::cerr << "Could not load unit logo texture from " << logoPath << "\n";
		}
		else
		{
			logoSprite.emplace(logoTexture);
		}
	}  
}

void Unit::Draw(sf::RenderWindow& window, float zoomFactor)
{
    // 
	float cellSize = TILE_SIZE * zoomFactor;
	sf::Vector2f cellCenter((gridX + 0.5f) * cellSize, (gridY + 0.5f) * cellSize);

	// Draw heatlh bar on top
	float barWidth = cellSize * 0.8f;
	float barHeight = 5.f * zoomFactor;
	float hpRatio = curHealth / maxHealth;

	sf::RectangleShape hpBg(sf::Vector2f(barWidth, barHeight));
	hpBg.setFillColor(sf::Color(200, 200, 200));
	hpBg.setOrigin(sf::Vector2f(barWidth / 2.f, barHeight / 2.f));
	hpBg.setPosition(sf::Vector2f(cellCenter.x, cellCenter.y - cellSize * 0.4f));
	window.draw(hpBg);

	sf::RectangleShape hpBar(sf::Vector2f(barWidth * hpRatio, barHeight));
	hpBar.setFillColor(sf::Color::Red);
	hpBar.setOrigin(sf::Vector2f(barWidth / 2.f * hpRatio, barHeight / 2.f));
	hpBar.setPosition(sf::Vector2f(cellCenter.x - (barWidth * (1- hpRatio) / 2.f), cellCenter.y - cellSize * 0.4f));
	window.draw(hpBar);

	// Draw unit shape
	float radius = cellSize * 0.35f;
	shape.setRadius(radius);
	shape.setOrigin(sf::Vector2f(radius, radius));
	shape.setPosition(sf::Vector2f(cellCenter.x, cellCenter.y + cellSize * 0.1f));
	window.draw(shape);
	
	// Draw unit logo
	if (logoTexture.getSize().x > 0 && logoSprite.has_value())
	{
		float maxLogoSize = radius * 1.5f;
		float logoScale = maxLogoSize / std::max(logoTexture.getSize().x, logoTexture.getSize().y);

		sf::Sprite& sprite = logoSprite.value();
		sprite.setScale(sf::Vector2f(logoScale, logoScale));
		sprite.setOrigin(sf::Vector2f(logoTexture.getSize().x / 2.f, logoTexture.getSize().y / 2.f));
		sprite.setPosition(shape.getPosition());
		window.draw(sprite);
	}
}

void Unit::MoveTo(int newX, int newY)
{
	gridX = newX;
	gridY = newY;
}

void Unit::Attack(Unit& target)
{

}

bool Unit::IsEnemy(const Unit& target) const
{
	return false;
}


