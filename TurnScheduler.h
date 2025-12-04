// TurnScheduler.h
#pragma once
#include <vector>
#include <random>
#include "Faction.h"
class TurnScheduler
{
public:
	std::vector<Unit*> turnOrder;
	int currentIndex = 0;

	void GenerateTurnOrder(const std::vector<Faction*>& factions, std::mt19937& rng);
	void RemoveDeadUnits();

	Unit* GetNextUnit();
};