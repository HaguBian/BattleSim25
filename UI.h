#pragma once
#include <SFML/Graphics.hpp>
#include "Unit.h"


inline sf::Font gFont;

int InitializeUI();

std::string FormatFloat(std::string value, int precision);

void DrawUnitInfoPanel(sf::RenderWindow& window, Unit * unit);
