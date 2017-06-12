#pragma once
#include"SFML\Graphics.hpp"
#include"MapDefs.h"
#include"UnitDefs.h"
#include"CameraDefs.h"
//Do not include GUIDefs in this file. Buildings should not interact with the GUI, the GUI should interact with buildings. 
class Building
{
public:
	GUIDependencies* guidep;
	sf::RenderWindow* wind;
	BuildingDependencies::BuildingStates currstate = BuildingDependencies::BuildingStates::IDLE;
	sf::Sprite visual;
	int visionSquare;
	map* gamemap;
	int tier;
	sf::Vector2i location;
	int tileheight;
	int tilewidth;
	int team;
	UnitDependencies::UnitFaction faction;
	double HP;
	double Armor;
	int IridiumCost;
	int KaskanCost;
	void provideVision(double delta);
	virtual void update(double delta) = 0;
	virtual void render(sf::RenderWindow* wind) = 0;
};
class ProductionBuilding : public  Building
{
public:
	UnitDependencies::UnitType currProducingUnitType = UnitDependencies::UnitType::NULLUNITTYPE;
	UnitDependencies* UnitDepend;
	bool sufficientIridium = false;
	bool sufficientKaskan = false;
	double origProductionRate;
	double productionRate;
	double currentlyProducedIridium = 0;
	double currentlyProducedKaskan = 0;
	//In this vector, unit types are pushed back by independent GUI elements. The production building simply goes through these types and builds based on their values.
	std::vector<UnitDependencies::UnitType> productionQueue;
	virtual void produce(double delta) = 0;
};
class UnitProductionBuilding : public ProductionBuilding
{
	void render(sf::RenderWindow* wind);
	void update(double delta);
	void produce(double delta);
	UnitProductionBuilding(int teamC,
		UnitDependencies::UnitFaction factionC,
		double HPC,
		double ArmorC,
		int IridiumCostC,
		int KaskanCostC,
		double productionRateC,
		map* gamemapC,
		int tierC,
		int tileHeightC,
		int tileWidthC,
		int visionRadiusC,
		sf::Vector2i locC,
		sf::Texture* tex,
		GUIDependencies* guidepC,
		UnitDependencies* unitdependC,
		sf::RenderWindow* windC);
};
class OffensiveBuilding : virtual Building
{
public:
	UnitDependencies::AttackType atktype;
	double damage;
	virtual void attack();
};
