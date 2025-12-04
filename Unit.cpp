// Unit.cpp
#include <iostream>
#include <cmath>
#include <algorithm>

#include "Unit.h"
#include "GridMap.h"
#include "Faction.h"
#include "UI.h"

Unit::Unit(int x, int y, int f, const UnitTemplate& templ)
{
	gridX = x;
	gridY = y;
	factionId = f;

	unitType = templ.type;
	unitName = templ.name;

	// Base stats
	base.maxHP = templ.hp;
	base.defense = templ.def;
	base.attack = templ.atk;
	base.penetration = templ.penetration;
	base.cooldown = templ.cooldown;
	base.morale = templ.morale;
	base.move = templ.mobility;
	base.range = templ.range;
	base.aoe = templ.aoe;
	base.regen = templ.regen;
	base.vision = templ.vision;
	base.utility = templ.utility;

	RecalculateFinalStats(true);

	// Initialize shape  
	float radius = TILE_SIZE / 2.f - 2.f;  
	shape.setRadius(radius);  
	shape.setOrigin(sf::Vector2f(radius, radius));  

	switch (factionId)  
	{  
	case 0: shape.setFillColor(sf::Color::Blue); break;  
	case 1: shape.setFillColor(sf::Color::Red); break;  
	case 2: shape.setFillColor(sf::Color::Green); break;  
	case 3: shape.setFillColor(sf::Color::Yellow); break;  
	default: shape.setFillColor(sf::Color::White); break;  
	}  

	shape.setPosition(sf::Vector2f(gridX * TILE_SIZE + TILE_SIZE / 2,  
									gridY * TILE_SIZE + TILE_SIZE / 2));  
}

void Unit::Draw(sf::RenderWindow& window, float zoomFactor)
{
    // 
	float cellSize = TILE_SIZE * zoomFactor;
	sf::Vector2f cellCenter((gridX + 0.5f) * cellSize, (gridY + 0.5f) * cellSize);

	// Draw heatlh bar on top
	float barWidth = cellSize * 0.8f;
	float barHeight = 5.f * zoomFactor;
	float hpRatio = final.curHP / final.maxHP;

	sf::RectangleShape hpBg(sf::Vector2f(barWidth, barHeight));
	hpBg.setFillColor(sf::Color(200, 200, 200));
	hpBg.setOrigin(sf::Vector2f(barWidth / 2.f, barHeight / 2.f));
	hpBg.setPosition(sf::Vector2f(cellCenter.x, cellCenter.y - cellSize * 0.4f));
	window.draw(hpBg);

	sf::RectangleShape hpBar(sf::Vector2f(barWidth * hpRatio, barHeight));
	hpBar.setFillColor(sf::Color::Red);
	hpBar.setOrigin(sf::Vector2f(barWidth / 2.f * hpRatio, barHeight / 2.f));
	hpBar.setPosition(sf::Vector2f(cellCenter.x - (barWidth * (1- hpRatio) / 2.f), cellCenter.y - cellSize * 0.4f));
	window.draw(hpBar);

	// Draw unit shape
	float radius = cellSize * 0.35f;
	shape.setRadius(radius);
	shape.setOrigin(sf::Vector2f(radius, radius));
	shape.setPosition(sf::Vector2f(cellCenter.x, cellCenter.y + cellSize * 0.1f));
	window.draw(shape);
	
	//// Draw unit LEVEL (centered on the unit)
	sf::Text levelText(gFont,"");
	levelText.setString(std::to_string(level));
	levelText.setCharacterSize(12);
	levelText.setFillColor(sf::Color::White);
	levelText.setOutlineColor(sf::Color::Black);
	levelText.setOutlineThickness(2.f);
	sf::FloatRect bounds = levelText.getLocalBounds();
	levelText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x * 0.5f,
									 bounds.position.y + bounds.size.y * 0.5f));
	levelText.setPosition(shape.getPosition());
	window.draw(levelText);
}

void Unit::MoveTo(int newX, int newY)
{
	if (!CanAct()) return;

	gridX = newX;
	gridY = newY;

	lastAction = ActionType::Move;
}

void Unit::Attack(Unit& target)
{
	if (!CanAct()) return;

	float effectiveDefense = std::max(0.f, target.final.defense - final.penetration);
	float dmg = std::max(0.f, final.attack - effectiveDefense);


	float oldHealth = target.final.curHP;
	target.final.curHP -= dmg;

	lastAction = ActionType::Attack;

	if (oldHealth > 0 && target.final.curHP <= 0)
	{
		GainExp(target.level);
	}
}

void Unit::Heal(Unit& ally)
{
	if (!CanAct()) return;
	float healAmount = final.maxHP * 0.2f;
	ally.final.curHP = std::min(ally.final.maxHP, ally.final.curHP + healAmount);
	lastAction = ActionType::Heal;
}

void Unit::Scout(int radius)
{
	if (!CanAct()) return;
	lastAction = ActionType::Scout;
}

void Unit::BuildStructure()
{
	if (!CanAct()) return;
	lastAction = ActionType::Build;
}

void Unit::UpdateVision(const std::vector<Unit*>& allUnits)
{
	visibleEnemyCache.clear();
	for (Unit* other : allUnits)
	{
		if (other == this) continue;
		if (other->final.curHP <= 0) continue;

		if (this->factionId == other->factionId) continue;

		if (Unit::Distance(this, other) <= final.vision)
		{
			visibleEnemyCache.push_back(other);
		}
	}
}

bool Unit::IsEnemyInVision(Unit* target) const
{
	return (Distance(this, target) <= final.vision);
}

bool Unit::IsEnemyVisibleShare(Faction* faction, Unit* enemy)
{
	return (std::find(faction->visibleEnemies.begin(), 
			faction->visibleEnemies.end(),
			enemy) != faction->visibleEnemies.end());
}

bool Unit::IsEnemyInAttackRange(Unit* enemy)
{
	return Distance(this, enemy) <= final.range;
}

void Unit::TakeTurn(Faction* faction, const std::vector<Unit*>& allUnits)
{
	faction->UpdateSharedVision(allUnits);

	Unit* target = nullptr;

	for (Unit* e : faction->visibleEnemies)
	{
		if (!target || Distance(this, e) < Distance(this, target))
		{
			target = e;
		}
	}

	if (!target)
	{
		//Patrol();
		return;
	}
	if (IsEnemyInAttackRange(target))
	{
		Attack(*target);
		return;
	}
	//MoveTowards(target);

}

bool Unit::CanAct() const
{
	return  lastAction == ActionType::None;
}

float Unit::Distance(const Unit* a, const Unit* b)
{
	float dx = float(a->gridX - b->gridX);
	float dy = float(a->gridY - b->gridY);
	return std::sqrt(dx * dx + dy * dy);
}

bool Unit::IsOccupied(int x, int y, const std::vector<Unit*>& allUnits)
{
	for (auto* u : allUnits)
		if (u->final.curHP > 0 && u->gridX == x && u->gridY == y)
			return true;
	return false;
}

void Unit::GainExp(int amount)
{
	if (level >= 10) return;

	currExp += amount;
	while (currExp >= expToLevelUp && level < 10)
	{
		currExp -= expToLevelUp;
		LevelUp();
	}

	if (level >= 10) currExp = 0;
}

void Unit::AddExperience(float amount)
{
	currExp += amount;
	while (currExp >= expToLevelUp && level < 10)
	{
		currExp -= expToLevelUp;
		LevelUp();
	}
}

void Unit::LevelUp()
{
	if (level >= 10) return;

	level++;
	statPoints++;
	totalPoints++;

	base.regen += base.maxHP * 0.01f;

	expToLevelUp = (1 << level); // 2^level

	std::cout << unitName << " reached level " << level << "!\n";

	RecalculateFinalStats(false);
}

bool Unit::SpendStatPoint(const std::string& stat)
{
	if (statPoints <= 0) return false;

	if (stat == "HP")
	{
		float oldMax = base.maxHP;
		base.maxHP += 1;
		float delta = base.maxHP - oldMax;
		final.curHP += delta;
	}
	else if (stat == "ATK") base.attack++;
	else if (stat == "DEF") base.defense++;
	else if (stat == "PEN") base.penetration++;
	else if (stat == "CD") base.cooldown++;
	else if (stat == "MORALE") base.morale++;
	else if (stat == "MOVE") base.move++;
	else if (stat == "RANGE") base.range++;
	else if (stat == "AOE") base.aoe++;
	else if (stat == "REGEN") base.regen++;
	else if (stat == "VISION") base.vision++;
	else if (stat == "UTILITY") base.utility++;
	else return false;

	statPoints--;
	RecalculateFinalStats(false);
	return true;
}

void Unit::RecalculateFinalStats(bool init)
{
	float hpPercent = 1.f;
	if (!init && final.maxHP > 0)
		hpPercent = final.curHP / final.maxHP;

	final.maxHP = base.maxHP * factor.hpFactor;
	final.defense = base.defense * factor.defenseFactor;
	final.attack = base.attack * factor.attackFactor;
	final.penetration = base.penetration * factor.penetrationFactor;
	final.cooldown = base.cooldown * factor.cooldownFactor;
	final.morale = base.morale * factor.moraleFactor;
	final.move = base.move * factor.moveFactor;
	final.range = base.range * factor.rangeFactor;
	final.aoe = base.aoe * factor.aoeFactor;
	final.regen = base.regen * factor.regenFactor;
	final.vision = base.vision * factor.visionFactor;
	final.utility = base.utility * factor.utilityFactor;


	final.curHP = final.maxHP * hpPercent;

	if (final.curHP > final.maxHP)
		final.curHP = final.maxHP;
}
