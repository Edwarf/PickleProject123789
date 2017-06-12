#include"stdafx.h"
#include"TeamDefs.h"
#include"UnitDefs.h"
#include"ObjectDependencyDefs.h"
#include"BuildingDefs.h"
Team::Team(UnitDependencies* unitdepC, TileDependencies* tiledepC, BuildingDependencies* buildingdepC, GUIDependencies* guidepC, map* gamemapC, int numberC, sf::RenderWindow* wind)
{
	unitdep = unitdepC;
	tiledep = tiledepC;
	buildingdep = buildingdepC;
	guidep = guidepC;
	gamemap = gamemapC;
	number = numberC;
	win = wind;
	TeamUnits.push_back(new T1BasicFighter(UnitDependencies::UnitType::LeopardTank, UnitDependencies::UnitFaction::Modern, TileDependencies::tileType::Flatland, gamemap, UnitDependencies::AttackType::Physical, UnitDependencies::LeopardTankHP,
		UnitDependencies::LeopardTankIridiumCost, UnitDependencies::LeopardTankKaskanCost, UnitDependencies::LeopardTankArmor, UnitDependencies::LeopardTankSpeed, sf::Vector2f(1,1)
		, &unitdep->LeopardTex, guidep, win, UnitDependencies::LeopardTankVisionRange, UnitDependencies::LeopardTankRange, UnitDependencies::LeopardTankRof, UnitDependencies::LeopardTankDamage));
}
void Team::update(double delta)
{
	for (int i = 0; i < TeamBuildings.size(); i++)
	{
		TeamBuildings[i]->update(delta);
	}
	for (int i = 0; i < TeamUnits.size(); i++)
	{
		TeamUnits[i]->update(delta);
	}
}
void Team::render(sf::RenderWindow* wind)
{
	for (int i = 0; i < TeamBuildings.size(); i++)
	{
		TeamBuildings[i]->render(wind);
	}
	for (int i = 0; i < TeamUnits.size(); i++)
	{
		TeamUnits[i]->render(wind);
	}
}
void Team::destroyMemory()
{
	for (int i = 0; i < TeamBuildings.size(); i++)
	{
		delete TeamBuildings[i];
	}
	for (int i = 0; i < TeamUnits.size(); i++)
	{
		delete TeamUnits[i];
	}
}