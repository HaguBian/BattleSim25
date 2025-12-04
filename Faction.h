//Faction.h
#pragma once
#include <vector>
#include <string>
#include "Unit.h"

class AIController; // forward declaration

class Faction
{
public:
    int factionID;
    std::string factionName;

    std::vector<Unit*> units;
	std::vector<Unit*> visibleEnemies;


    AIController* aiController;

    Faction(int id, const std::string& name);
    ~Faction();

	void DrawVision(sf::RenderWindow& window, float zoomFactor);
    void AddUnit(Unit* unit);
    bool OwnsUnit(const Unit* unit) const;
	void UpdateSharedVision(const std::vector<Unit*>& allUnits);

    void TakeTurn(const std::vector<Unit*>& allUnits);
};
