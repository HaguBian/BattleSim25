#include "UI.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

int InitializeUI()
{
	if (!gFont.openFromFile("resources/fonts/Orbitron-VariableFont_wght.ttf"))
	{
		std::cerr << "Could not load font\n";
		return -1;
	}
	return 0;
}

std::string FormatFloat(std::string value, int precision = 2)
{
	std::stringstream ss;

	float absVal = std::stof(value);
	std::string suffix;

	if (absVal >= 1'000'000'000'000.f) // T
	{
		absVal /= 1'000'000'000'000.f;
		suffix = "T";
	}
	else if (absVal >= 1'000'000'000.f) // B
	{
		absVal /= 1'000'000'000.f;
		suffix = "B";
	}
	else if (absVal >= 1'000'000.f) // M
	{
		absVal /= 1'000'000.f;
		suffix = "M";
	}
	else if (absVal >= 1'000.f) // K
	{
		absVal /= 1'000.f;
		suffix = "K";
	}

	ss << std::fixed << std::setprecision(precision) << absVal << suffix;
	return ss.str();
}

void DrawUnitInfoPanel(sf::RenderWindow& window, Unit* unit)
{
	if (unit == nullptr) return;

	float panelWidth = 400.f;
	float panelHeight = 600.f;

	sf::Vector2f panelPos(window.getSize().x - panelWidth - 50.f, 50.f);

	// Panel background
	sf::RectangleShape panel(sf::Vector2f(panelWidth, panelHeight));
	panel.setFillColor(sf::Color(50, 50, 50, 150));
	panel.setOutlineColor(sf::Color::White);
	panel.setOutlineThickness(2.f);
	panel.setPosition(panelPos);
	window.draw(panel);

	float padding = 10.f;
	float x = panelPos.x + padding;
	float y = panelPos.y + padding;

	// Unit name
	sf::Text nameText(gFont, unit->unitName, 32);
	sf::FloatRect nameBounds = nameText.getLocalBounds();
	nameText.setOrigin(sf::Vector2f(nameBounds.position.x + nameBounds.size.x / 2.f, 0.f));
	nameText.setPosition(sf::Vector2f(x + panelWidth / 2.f, y));
	nameText.setFillColor(sf::Color::White);
	window.draw(nameText);
	y += 40.f;
	
	// Unit type
	sf::Text typeText(gFont, unit->unitType, 18);
	sf::FloatRect typeBounds = typeText.getLocalBounds();
	typeText.setOrigin(sf::Vector2f(typeBounds.position.x + typeBounds.size.x / 2.f, 0.f));
	typeText.setPosition(sf::Vector2f(x + panelWidth / 2.f, y));
	typeText.setFillColor(sf::Color::White);
	window.draw(typeText);
	y += 30.f;

	// Health
	float barWidth = panelWidth - 2 * padding;
	float barHeight = 20.f;
	sf::RectangleShape hpBg(sf::Vector2f(barWidth, barHeight));
	hpBg.setFillColor(sf::Color(200, 200, 200));
	hpBg.setPosition(sf::Vector2f(x, y));
	window.draw(hpBg);

	float hpPercent = unit->curHealth / unit->maxHealth;
	hpPercent = std::max(0.f, std::min(1.f, hpPercent));

	sf::RectangleShape hpBar(sf::Vector2f(barWidth * hpPercent, barHeight));
	hpBar.setFillColor(sf::Color::Red);
	hpBar.setPosition(sf::Vector2f(x, y));
	window.draw(hpBar);

	sf::Text hpText(gFont, "HP: " + std::to_string(int(unit->curHealth)) + " / " + std::to_string(int(unit->maxHealth)), 12);
	hpText.setPosition(sf::Vector2f(x + 5.f, y + 2.f));
	hpText.setFillColor(sf::Color::Black);
	window.draw(hpText);

	y += 30.f;

	// Stats
	float col1X = x;
	float col2X = x + panelWidth / 2.f;

	struct Row { std::string k; std::string v; };
	std::vector<Row> stats = {
		{ "ATK", std::to_string(unit->armor) },
		{ "DEF",     std::to_string(unit->shield) },
		{ "MOVE",    std::to_string(unit->defense) },
		{ "HIT",     std::to_string(unit->shield) + "%" },
	};

	for (auto& r : stats)
	{
		sf::Text left(gFont, r.k, 14);
		left.setPosition(sf::Vector2f(col1X, y));
		left.setFillColor(sf::Color::White);
		window.draw(left);

		sf::Text right(gFont, FormatFloat(r.v, 2), 14);
		right.setPosition(sf::Vector2f(col2X, y));
		right.setFillColor(sf::Color::White);
		window.draw(right);
		y += 20.f;
	}
}
