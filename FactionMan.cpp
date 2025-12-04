// FactionMan.cpp
#include "FactionMan.h"

void FactionManager::AddFaction(Faction* faction)
{
	factions.push_back(faction);
}

void FactionManager::BuildTurnOrder()
{
	scheduler.GenerateTurnOrder(factions, rng);
}

void FactionManager::RemoveDeadUnits()
{
    for (auto* faction : factions)
    {
        faction->units.erase(
            std::remove_if(
                faction->units.begin(),
                faction->units.end(),
                [](Unit* u) { return u->final.curHP <= 0; }
            ),
            faction->units.end()
        );
    }

    scheduler.RemoveDeadUnits();
}


std::vector<Unit*> FactionManager::GetAllUnits() const
{
    std::vector<Unit*> all;
    for (auto* f : factions)
        all.insert(all.end(), f->units.begin(), f->units.end());
    return all;
}

void FactionManager::NextTurn()
{
    RemoveDeadUnits();

    std::vector<Unit*> allUnits;
    for (auto* f : factions)
        allUnits.insert(allUnits.end(), f->units.begin(), f->units.end());

    Unit* unit = scheduler.GetNextUnit();
    if (!unit || unit->final.curHP <= 0) return;

    // Reset action
    unit->lastAction = ActionType::None;

    Faction* fac = unit->faction;

    if (fac && fac->aiController)
        fac->aiController->TakeTurn(unit, allUnits);
}
