
#pragma once

#include<SFML\Graphics.hpp>
#include"ObjectDependencyDefs.h"
#include"MapDefs.h"
#include"GUIDefs.h"
sf::Vector2f normalize(const sf::Vector2f& source);

class Unit
{
public:
	//Constructor Values
	//type should be class name 
	int team;
	UnitDependencies::UnitType type;
	UnitDependencies::UnitFaction faction;
	UnitDependencies::UnitStates currstate;
	UnitDependencies::UnitStates laststate;
	Unit();
	TileDependencies::tileType availabletyles;
	//Present to implement amphibious units. If one terrain is desired set availabletyles2 to the value of availabletyles
	TileDependencies::tileType availabletyles2;
	//Returns all available tile directions
	map* gamemap;
	UnitDependencies::AttackType atktype;
	int HP;
	int IridiumCost;
	int KaskanCost;
	int Armor;
	int Speed;
	sf::Vector2f positionfloat;
	Collection UnitUI;
	//Radius is in tiles, converted to pixles in constructor
	int visionSideSize;

	//Internal Values
	sf::Sprite visual;
	sf::Vector2f LastMoveInterval;
	//Don't use positionfloat when dealing with game map lists
	sf::Vector2f desiredpos;
	std::vector<sf::Vector2f> pathCheck(double delta);
	void CustomMove(double delta);
	void render(sf::RenderWindow* wind);
	void update(double delta);
	void turn(double delta);
	void provideVision(double delta);
	sf::Vector2f last_desired_pos;
	void readjustDesiredPos(double delta);
	void DefaultUISetup(GUIDependencies*, sf::RenderWindow* wind);
	virtual void performAction(double delta) = 0;
};
class LandUnit : public Unit

{

public:
	int range;
	double Rof;
	LandUnit();
	//usually attacks if on other unit, moves if not
};
class Engineer
{
	double productionSpeed;
	double origProductionSpeed;
};
//In t1engineers, RoF is 
class T1Engineer : public Engineer
{
	void performAction(double delta);
	void construct(double delta);
	T1Engineer(UnitDependencies::UnitType typeC,
		UnitDependencies::UnitFaction factionC,
		TileDependencies::tileType availabletylesC,
		//Returns all available tile directions
		map* gamemapC,
		UnitDependencies::AttackType atktypeC,
		const int HPC,
		const int IridiumCostC,
		const int KaskanCostC,
		const int ArmorC,
		int SpeedC,
		sf::Vector2f currentPosition,
		sf::Texture* tex,
		GUIDependencies* GUIDep,
		sf::RenderWindow* wind,
		const int visionRadiusC,
		const int rangeC,
		double productionSpeedC,
		double origProductionSpeedC
	);
};
class CombatUnit : public LandUnit
{
public:
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
	int HPC,
	int IridiumCostC,
	int KaskanCostC,
	const int ArmorC,
	int SpeedC,
	sf::Vector2f currentPosition,
	sf::Texture* tex,
	GUIDependencies* GUIDep,
	sf::RenderWindow* wind,
	int visionRadiusC,
	int rangeC,
	double RofC,
	double damageC
	);
};
//BASIC FIGHTER, T1 
