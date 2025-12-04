// UnitTemplate.h
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

struct UnitTemplate
{
	std::string name;
	std::string type;
	int atk;
	int penetration;
	int aoe;

	int hp;
	int def;
	int morale;

	int cooldown;
	int range;
	int mobility;

	int regen;
	int vision;
	int utility;

	bool isValid() const
	{
		int total = atk + penetration + aoe +
					hp + def + morale +
					cooldown + range + mobility +
					regen + vision + utility;
		return total <= 50; // 40 + 10
	}
};

struct StatFactors
{
	static constexpr float HP_FACTOR = 100.0f;
	static constexpr float ATK_FACTOR = 10.0f;

    static constexpr float VISION_FACTOR = 2.0f;
    static constexpr float RANGE_FACTOR = 1.0f;



    static constexpr float PENETRATION_FACTOR = 1.0f;
    static constexpr float AOE_FACTOR = 1.0f;
    static constexpr float DEF_FACTOR = 1.0f;
    static constexpr float MORALE_FACTOR = 1.0f;
    static constexpr float SPEED_FACTOR = 1.0f;
    static constexpr float MOBILITY_FACTOR = 1.0f;
    static constexpr float REGEN_FACTOR = 1.0f;
    static constexpr float UTILITY_FACTOR = 1.0f;
};