// Unit.h
#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>

#include "UnitTemplate.h"

class Faction; // forward declaration

enum class ActionType
{
	None,
	Move,
	Attack,
	Heal,
	Scout,
	Build,
	Skip
};

struct BaseStats {  
   float maxHP;          // Base HP  
   float defense;        // Base Defense  
   float attack;         // Base Attack  
   float penetration;    // Armor Penetration  
   float cooldown;       // Cooldown Time  
   float morale;         // Morale  
   float move;           // Movement Range  
   float range;          // Attack Range  
   float aoe;            // Area of Effect Radius  
   float regen;          // Turn-based Regeneration  
   float vision;         // Vision Range  
   float utility;        // Utility Index  
};

struct FinalStats {
	float curHP;
	float maxHP;
	float defense;
	float attack;
	float penetration;
	float cooldown;
	float morale;
	float move;
	float range;
	float aoe;
	float regen;
	float vision;
	float utility;
};

struct StatFactor {
	float hpFactor = 100.0f;		// 100
	float defenseFactor = 5.0f;		// 5
	float attackFactor = 10.0f;		// 10
	float penetrationFactor = 5.0f;	// 5
	float cooldownFactor = 1.0f;	// 
	float moraleFactor = 1.0f;		// 10
	float moveFactor = 1.0f;		// 1
	float rangeFactor = 1.0f;		// 1
	float aoeFactor = 1.0f;			// 1
	float regenFactor = 1.0f;		// 
	float visionFactor = 2.0f;		// 2
	float utilityFactor = 1.0f;		//
};

class Unit
{
public:
	// Grid position
	int gridX;
	int gridY;

	// Owner
	int factionId;
	Faction* faction; 

	// Indentity
	std::string unitType;
	std::string unitName;

	// Stats
	BaseStats base;
	FinalStats final;
	StatFactor factor;

	// Level and experience
	int level = 1;
	int currExp = 0;
	int expToLevelUp = 2;     // 2^1
	int totalPoints = 0;
	int statPoints = 0;

	bool isDead = false;
	std::vector<Unit*> visibleEnemyCache;
	ActionType lastAction = ActionType::None;

	// Rendering
	sf::CircleShape shape;

	// Constructor
	Unit(int x, int y, int f, const UnitTemplate& templ);
	virtual ~Unit() = default;

	// Rendering
	void Draw(sf::RenderWindow& window, float zoomLevel);

	// Main actions
	void MoveTo(int newX, int newY);
	void Attack(Unit& target);
	void Heal(Unit& ally);
	void Scout(int radius);
	void BuildStructure();
	void UpdateVision(const std::vector<Unit*>& allUnits);
	void TakeTurn(Faction* faction, const std::vector<Unit*>& allUnits);

	// Level / Exp
	void GainExp(int amount);
	void AddExperience(float amount);
	void LevelUp();

	// Stat system
	bool SpendStatPoint(const std::string& statName);

	// Recalculate final stats
	void RecalculateFinalStats(bool init);

	// Checks
	bool IsEnemyInVision(Unit* target) const;
	bool IsEnemyVisibleShare(Faction* faction, Unit* enemy);
	bool IsEnemyInAttackRange(Unit* enemy);
	bool CanAct() const;
	
	// Static helpers
	static bool IsOccupied(int x, int y, const std::vector<Unit*>& allUnits);
	static float Distance(const Unit* a, const Unit* b);
};