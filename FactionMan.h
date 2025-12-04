// FactionMan.h
#pragma once
#include <vector>
#include <random>
#include "Faction.h"
#include "AIController.h"
#include "TurnScheduler.h"

class FactionManager
{
public:
	std::vector<Faction*> factions;
	
	std::random_device rd;
	std::mt19937 rng;

	TurnScheduler scheduler;

	// Constructor
	FactionManager() : rng(rd()) {}

	void AddFaction(Faction* faction);
	void NextTurn();
	void BuildTurnOrder();
	void RemoveDeadUnits();

	std::vector<Unit*> GetAllUnits() const;
};
