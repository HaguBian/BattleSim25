#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

enum class UnitCategory { 
	Infantry, // 0
};

class Unit
{
public:
	// Grid position
	int gridX;
	int gridY;

	// Unit info
	int faction; // 0, 1, 2, ...
	UnitCategory category;
	std::string unitType = "";
	std::string unitName = "";

	// Core stats
	float curHealth;
	float maxHealth;
	float defense;
	float shield;
	float armor;
	
	// Unit shape
	sf::CircleShape shape;
	sf::Texture logoTexture;
	std::optional<sf::Sprite> logoSprite;

	// Constructor
	Unit(int x, int y, int f,
		 float def, float shd, float arm,
		 const std::string& logoPath = ""
		);

	virtual ~Unit() = default;

	// Methods
	void Draw(sf::RenderWindow& window, float zoomLevel);
	void MoveTo(int newX, int newY);
	void Attack(Unit& target);
	bool IsEnemy(const Unit& target) const;


};