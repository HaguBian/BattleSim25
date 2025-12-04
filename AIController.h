//AIController.h
#pragma once
#include <random>
#include <iostream>

#include "Faction.h"

class AIController
{
public:
	Faction* faction;
	std::random_device rd;
	std::mt19937 rng;

	AIController(Faction* f) : faction(f), rng(rd()) {}
	virtual ~AIController() = default;
	virtual void TakeTurn(Unit* unit);
	void TakeTurn(Unit* unit, const std::vector<Unit*>& allUnits);
};