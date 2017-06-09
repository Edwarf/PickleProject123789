
#pragma once

#include<SFML\Graphics.hpp>

#include"MapDefs.h"

sf::Vector2f normalize(const sf::Vector2f& source);

class UnitDependencies

{

public:
	enum AttackType

	{

		None,

		Physical,

		Laser,

	};
	enum TileDirection
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		UPDIAGNOLRIGHT,
		UPDIAGNOLLEFT,
		DOWNDIAGNOLRIGHT,
		DOWNDIAGNOLLEFT,
		//Stationary denotes to the same tile 
		STATIONARY
	};
	enum UnitStates
	{
		IDLE,
		MOVING,
		ATTACKING,
		REPOSITIONING,
	};
	enum UnitFaction
	{
		Modern
	};
	//Associates itself with a defined class
	enum UnitType
	{
		LeopardTank,


	};
	sf::Texture LeopardTex;
	UnitDependencies();
};
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
	//Returns all available tile directions
	map* gamemap;
	UnitDependencies::AttackType atktype;
	double HP;
	int IridiumCost;
	int KaskanCost;
	double Armor;
	double Speed;
	sf::Vector2f positionfloat;

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
	sf::Vector2f last_desired_pos;
	void readjustDesiredPos(double delta);

};
class LandUnit : public Unit

{

public:
	int range;
	int Rof;
	LandUnit();

	//usually attacks if on other unit, moves if not

	virtual void interact() = 0;

};
class T1BasicFighter : public LandUnit

{

public:
	//Constructor Values


	void interact();

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
	double SpeedC,
	sf::Vector2f currentPosition,
	sf::Texture* tex);

};


//BASIC FIGHTER, T1 
