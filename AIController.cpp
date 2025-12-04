//AIController.h
#include <iostream>

#include "AIController.h"
#include "Gridmap.h"

void AIController::TakeTurn(Unit* unit, const std::vector<Unit*>& allUnits)
{
    if (!unit || !unit->CanAct()) return;

    unit->faction->UpdateSharedVision(allUnits);

    Unit* targetInRange = nullptr;
    Unit* nearestVisibleEnemy = nullptr;
    float nearestDist = 1e9f;

	//  Find nearest visible enemy and any in attack range
    for (Unit* e : unit->faction->visibleEnemies)
    {
        float d = Unit::Distance(unit, e);

        if (unit->IsEnemyInAttackRange(e))
        {
            targetInRange = e;
            break;
        }

        if (!nearestVisibleEnemy || d < nearestDist)
        {
            nearestVisibleEnemy = e;
            nearestDist = d;
        }
    }

	// Attack if enemy in range
    if (targetInRange)
    {
        unit->Attack(*targetInRange);
        return;
    }

	// Move to nearest visible enemy
    if (nearestVisibleEnemy)
    {
        int dx = nearestVisibleEnemy->gridX - unit->gridX;
        int dy = nearestVisibleEnemy->gridY - unit->gridY;

        int stepX = (dx != 0 ? dx / std::abs(dx) : 0);
        int stepY = (dy != 0 ? dy / std::abs(dy) : 0);

        int nx = unit->gridX + stepX;
        int ny = unit->gridY + stepY;

        if (!GridMap::InBounds(nx, ny))
        {
            return;
        }

        if (Unit::IsOccupied(nx, ny, allUnits))
        {
            return;
        }

        unit->MoveTo(nx, ny);
        return;
    }

	// Move randomly (wander)
    std::vector<std::pair<int, int>> dirs = { {1,0},{-1,0},{0,1},{0,-1} };
    std::shuffle(dirs.begin(), dirs.end(), rng);

    for (auto& d : dirs)
    {
        int nx = unit->gridX + d.first;
        int ny = unit->gridY + d.second;

        if (!GridMap::InBounds(nx, ny)) continue;
        if (Unit::IsOccupied(nx, ny, allUnits)) continue;
        unit->MoveTo(nx, ny);
        return;
    }

	// No valid moves
    unit->lastAction = ActionType::Skip;
    return;
}



void AIController::TakeTurn(Unit* unit)
{ 
    
}
