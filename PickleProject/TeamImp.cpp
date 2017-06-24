#include"stdafx.h"
#include"TeamDefs.h"
#include"UnitDefs.h"
#include"ObjectDependencyDefs.h"
#include"BuildingDefs.h"
Team::Team(UnitDependencies* unitdepC, TileDependencies* tiledepC, BuildingDependencies* buildingdepC, GUIDependencies* guidepC, ProjectileDependencies* projectdependC, map* gamemapC, int numberC, sf::RenderWindow* wind, MouseState* mouseC)
{
	totalIridium = 10000;
	totalKaskan = 10000;
	IridiumCapacity = 10000;
	KaskanCapacity = 10000;
	unitdep = unitdepC;
	tiledep = tiledepC;
	buildingdep = buildingdepC;
	guidep = guidepC;
	gamemap = gamemapC;
	number = numberC;
	win = wind;
	mouse = mouseC;
	projectdepend = projectdependC;
	TeamUnits.push_back(new Commander(UnitDependencies::UnitType::ModernCommander, UnitDependencies::UnitFaction::Modern, TileDependencies::tileType::Flatland, TileDependencies::tileType::Aquatic, gamemap, UnitDependencies::ModernCommanderAttackType, UnitDependencies::ModernCommanderHP, 0, 0, UnitDependencies::ModernCommanderArmor, UnitDependencies::ModernCommanderSpeed, UnitDependencies::ModernCommanderRof,
		sf::Vector2f(number*5, 10), &unitdep->ModernCommanderTex, guidep, wind, UnitDependencies::ModernCommanderVisionRange, UnitDependencies::ModernCommanderRange, UnitDependencies::ModernCommanderProductionRate, number, buildingdep, unitdep, projectdepend, mouse, UnitDependencies::ModernCommanderDamage));
	/*TeamBuildings.push_back(new UnitProductionBuilding(number, UnitDependencies::UnitFaction::Modern, BuildingDependencies::ModernFactoryHP, BuildingDependencies::ModernFactoryArmor, BuildingDependencies::ModernFactoryIridiumCost, BuildingDependencies::ModernFactoryKaskanCost,
	BuildingDependencies::ModernFactoryProductionRate, gamemap, BuildingDependencies::ModernFactoryTier, BuildingDependencies::ModernFactoryTileHeight, BuildingDependencies::ModernFactoryTileWidth, BuildingDependencies::ModernFactoryVisionRange, sf::Vector2i(number*5, 10), &buildingdep->ModernFactoryTex,
	guidep, unitdep, buildingdep, wind, guidep->GetModernFactoryUI(wind, unitdep), BuildingDependencies::BuildingType::ModernFactory, mouse, projectdepend));*/
}
void Team::update(double delta)
{
	for (int i = 0; i < TeamBuildings.size(); i++)
	{
		TeamBuildings[i]->update(delta);
		if (TeamBuildings[i]->currstate == BuildingDependencies::BuildingStates::DEAD)
		{
			delete TeamBuildings[i];
			TeamBuildings.erase(TeamBuildings.begin() + i);
		}
	}
	for (int i = 0; i < DeadUnits.size(); i++)
	{
		if (DeadUnits[i] != nullptr)
		{
			DeadUnits[i]->Dispose();
		}
	}
	for (int i = 0; i < TeamUnits.size(); i++)
	{
		TeamUnits[i]->update(delta);
		if (TeamUnits[i]->currstate == UnitDependencies::UnitStates::DEAD)
		{
			DeadUnits.push_back(TeamUnits[i]);
			TeamUnits.erase(TeamUnits.begin() + i);
		}
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