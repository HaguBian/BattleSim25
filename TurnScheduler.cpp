#include "TurnScheduler.h"
#include <algorithm>
#include <iostream>

void TurnScheduler::GenerateTurnOrder(const std::vector<Faction*>& factions, std::mt19937& rng)
{
    turnOrder.clear();
    currentIndex = 0;

    std::vector<std::vector<Unit*>> factionUnits;
    for (auto* f : factions)
    {
        std::vector<Unit*> aliveUnits;
        for (auto* u : f->units)
            if (u->final.curHP > 0)
                aliveUnits.push_back(u);

        std::shuffle(aliveUnits.begin(), aliveUnits.end(), rng);
        factionUnits.push_back(aliveUnits);
    }

    // Round-robin merge
    bool hasUnits = true;
    while (hasUnits)
    {
        hasUnits = false;
        for (auto& list : factionUnits)
        {
            if (!list.empty())
            {
                turnOrder.push_back(list.back());
                list.pop_back();
                hasUnits = true;
            }
        }
    }
}

Unit* TurnScheduler::GetNextUnit()
{
    if (turnOrder.empty()) return nullptr;

    size_t checked = 0;
    while (checked < turnOrder.size())
    {
        Unit* u = turnOrder[currentIndex];
        currentIndex = (currentIndex + 1) % turnOrder.size();

        if (u && u->final.curHP > 0)
            return u;

        checked++;
    }
    return nullptr;
}

void TurnScheduler::RemoveDeadUnits()
{
    turnOrder.erase(
        std::remove_if(
            turnOrder.begin(),
            turnOrder.end(),
            [](Unit* u) { return u->final.curHP <= 0; }
        ),
        turnOrder.end()
    );

    if (currentIndex >= turnOrder.size())
        currentIndex = 0;
}

