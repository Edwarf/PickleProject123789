#pragma once
#include"SFML\Graphics.hpp"
#include"MapDefs.h"
#include"UnitDefs.h"
#include"CameraDefs.h"
//Do not include GUIDefs in this file. Buildings should not interact with the GUI, the GUI should interact with buildings. 
class BuildingDependencies
{
	enum BuildingStates
	{
		IDLE,
		//Constructing state is exclusively for production buildings
		ACTING,

	};
};
class Building
{
public:
	int team;
	UnitDependencies::UnitFaction faction;
	double HP;
	double Armor;
	int IridiumCost;
	int KaskanCost;
	virtual void update(double delta) = 0;
	virtual void render(sf::RenderWindow* wind) = 0;
};
class ProductionBuilding : public virtual Building
{
	double productionRate;
public:
	//In this vector, unit types are pushed back by independent GUI elements. The production building simply goes through these types and builds based on their values.
	std::vector<UnitDependencies::UnitType> productionQueue;
	virtual void produce();
};
class OffensiveBuilding : virtual public Building
{
public:
	UnitDependencies::AttackType atktype;
	double damage;
	virtual void attack();
};
