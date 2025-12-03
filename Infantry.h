#pragma once
#include "Unit.h"

class Swordsman : public Unit
{
public:
	Swordsman(int x, int y, int f)
		: Unit(x, y, f, 10.f, 5.f, 3.f, "")
	{
		category = UnitCategory::Infantry;
		unitName = "Swordsman";
		unitType = "Infantry";
	}
};