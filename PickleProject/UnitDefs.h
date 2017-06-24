
#pragma once

#include<SFML\Graphics.hpp>
#include"ObjectDependencyDefs.h"
#include"MapDefs.h"
#include"GUIDefs.h"
sf::Vector2f normalize(const sf::Vector2f& source);
class Projectile;
class Unit
{
public:
	//Potential unit being chased
	Unit* linkedUnit = nullptr;
	Building* linkedBuilding = nullptr;
	int range;
	//Constructor Values
	//type should be class name 
	int team;
	UnitDependencies::UnitType type;
	UnitDependencies::UnitFaction faction;
	UnitDependencies::UnitStates currstate;
	UnitDependencies::UnitStates laststate;
	TileDependencies::tileType availabletyles;
	//Present to implement amphibious units. If one terrain is desired set availabletyles2 to the value of availabletyles
	TileDependencies::tileType availabletyles2;
	//Returns all available tile directions
	map* gamemap;
	UnitDependencies::AttackType atktype;
	sf::RenderWindow* wind;
	double HP;
	int IridiumCost;
	int KaskanCost;
	double Armor;
	int Speed;
	sf::Vector2f positionfloat;
	MouseState* mouse;
	Collection UnitUI;
	//Radius is in tiles, converted to pixles in constructor
	int visionSideSize;
	BuildingDependencies* builddepend;
	GUIDependencies* GUIDep;
	UnitDependencies* unitdepend;
	ProjectileDependencies* projectdepend;
	//Internal Values
	sf::Sprite visual;
	sf::Vector2f LastMoveInterval;
	//Don't use positionfloat when dealing with game map lists
	sf::Vector2f desiredpos;
	std::vector<sf::Vector2f> pathCheck(double delta);
	void CustomMove(double delta);
	virtual void render(sf::RenderWindow* win);
	virtual void update(double delta);
	void turn(double delta);
	void provideVision(double delta);
	sf::Vector2f last_desired_pos;
	void readjustDesiredPos(double delta);
	virtual void CustomUISetup();
	void realign(double delta);
	void DefaultUISetup(GUIDependencies*, BuildingDependencies*,sf::RenderWindow* wind);
	virtual void performAction(double delta) = 0;
	virtual void Dispose();
};
class LandUnit : public virtual Unit

{

public:
	double Rof;
	//usually attacks if on other unit, moves if not
};
class Engineer : public virtual Unit
{

public:
	void CustomUISetup();
	PhantomBuilding* currconstruction;
	PhantomBuilding* lastconstruction;
	BuildingDependencies::BuildingType currBuildingType;
	double currentlyProducedKaskan;
	double currentlyProducedIridium;
	bool sufficientKaskan;
	bool sufficientIridium;
	double productionSpeed;
	double origProductionSpeed;
};
//In t1engineers, RoF is 
class T1Engineer : public Engineer
{

public:
	void performAction(double delta);
	T1Engineer(UnitDependencies::UnitType typeC,
		UnitDependencies::UnitFaction factionC,
		TileDependencies::tileType availabletylesC,
		TileDependencies::tileType availabletyles2C,
		//Returns all available tile directions
		map* gamemapC,
		UnitDependencies::AttackType atktypeC,
		double HPC,
		int IridiumCostC,
		int KaskanCostC,
		double ArmorC,
		int SpeedC,
		sf::Vector2f currentPosition,
		sf::Texture* tex,
		GUIDependencies* GUIDepC,
		sf::RenderWindow* wind,
		int visionRadiusC,
		int rangeC,
		double productionSpeedC,
		int teamC,
		BuildingDependencies* builddepend,
		UnitDependencies* unitdependC,
		ProjectileDependencies* projectdependC,
		MouseState* mouseC
		);
};
class CombatUnit : public LandUnit
{
public:
	virtual void render(sf::RenderWindow* win);
	virtual void update(double delta);
	void attack(double delta);
	double currRoF;
	std::vector<Projectile*> projectileList;
	virtual void performAction(double delta) = 0;
	double damage;
};
class T1BasicFighter : public CombatUnit
{
public:
	void performAction(double delta);
	//Constructor Values
	T1BasicFighter(UnitDependencies::UnitType typeC,
	UnitDependencies::UnitFaction factionC,
	TileDependencies::tileType availabletylesC,
	//Returns all available tile directions
	map* gamemapC,
	UnitDependencies::AttackType atktypeC,
	double HPC,
	int IridiumCostC,
	int KaskanCostC,
	double ArmorC,
	int SpeedC,
	sf::Vector2f currentPosition,
	sf::Texture* tex,
	GUIDependencies* GUIDep,
	sf::RenderWindow* wind,
	int visionRadiusC,
	int rangeC,
	double RofC,
	double damageC,
	BuildingDependencies* builddependC,
	ProjectileDependencies* projectdependC,
	int teamC,
	MouseState* mouseC
	);
};
//BASIC FIGHTER, T1 
class Commander : public virtual CombatUnit, public virtual Engineer
{
public:
	void render(sf::RenderWindow* win);
	void update(double delta);
	void performAction(double delta);
	Commander(UnitDependencies::UnitType typeC,
		UnitDependencies::UnitFaction factionC,
		TileDependencies::tileType availabletylesC,
		TileDependencies::tileType availabletyles2C,
		//Returns all available tile directions
		map* gamemapC,
		UnitDependencies::AttackType atktypeC,
		double HPC,
		int IridiumCostC,
		int KaskanCostC,
		double ArmorC,
		int SpeedC,
		double RofC,
		sf::Vector2f currentPosition,
		sf::Texture* tex,
		GUIDependencies* GUIDepC,
		sf::RenderWindow* wind,
		const int visionRadiusC,
		const int rangeC,
		double productionSpeedC,
		int teamC,
		BuildingDependencies* builddepend,
		UnitDependencies* unitdependC,
		ProjectileDependencies* projectdependC,
		MouseState* mouseC,
		double damageC
		);
};