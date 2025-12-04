// Faction.cpp
#include "Faction.h"
#include "AIController.h"
#include "GridMap.h"  

Faction::Faction(int id, const std::string& name)
    : factionID(id), factionName(name), aiController(nullptr)
{
}

Faction::~Faction()
{
    for (auto* unit : units)
        delete unit;
    if (aiController)
        delete aiController;
}

void Faction::DrawVision(sf::RenderWindow& window, float zoomFactor)
{
    float cellSize = TILE_SIZE * zoomFactor;
    sf::RectangleShape rect(sf::Vector2f(cellSize, cellSize));
    rect.setFillColor(sf::Color(255, 255, 255, 10)); 

    for (auto* u : units)
    {
        if (u->final.curHP <= 0) continue;

        int r = int(u->final.vision);
        for (int dx = -r; dx <= r; ++dx)
        {
            for (int dy = -r; dy <= r; ++dy)
            {
                int nx = u->gridX + dx;
                int ny = u->gridY + dy;
                if (nx < 0 || ny < 0 || nx >= GRID_WIDTH || ny >= GRID_HEIGHT) continue;
                if (dx * dx + dy * dy <= r * r)
                {
                    rect.setPosition(sf::Vector2f(nx * cellSize, ny * cellSize));
                    window.draw(rect);
                }
            }
        }
    }
}

void Faction::AddUnit(Unit* unit)
{
    units.push_back(unit);
}

bool Faction::OwnsUnit(const Unit* unit) const
{
    for (auto* u : units)
        if (u == unit) return true;
    return false;
}

void Faction::UpdateSharedVision(const std::vector<Unit*>& allUnits)
{
	visibleEnemies.clear();

    for (Unit* u : units)
    {
        if (u->final.curHP <= 0) continue;

        u->UpdateVision(allUnits);
        for (Unit* enemy : u->visibleEnemyCache)
        {
            if (std::find(visibleEnemies.begin(), visibleEnemies.end(), enemy) == visibleEnemies.end())
            {
                visibleEnemies.push_back(enemy);
            }
        }
    }
}

void Faction::TakeTurn(const std::vector<Unit*>& allUnits)
{
    if (aiController)
    {
        for (Unit* u : units)
            aiController->TakeTurn(u, allUnits);
    }
}
