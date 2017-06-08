
#pragma once

#include<SFML\Graphics.hpp>

#include"MapDefs.h"

sf::Vector2f normalize(const sf::Vector2f& source);

class UnitDependencies

{

public:
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

	};

	enum UnitType

	{



	};

	sf::Texture LeopardTex;

	UnitDependencies();

};

class Unit : public sf::Transformable

{



public:

	UnitDependencies::UnitStates currstate;

	Unit();

	TileDependencies::tileType availabletyles;
	//Returns all available tile directions
	std::vector<UnitDependencies::TileDirection> pathCheck(double delta);
	map* gamemap;

	enum AttackType

	{

		None,

		Physical,

		Laser,

	};

	AttackType atktype;

	double HP;

	double Armor;

	double Speed;

	sf::Sprite visual;

	//Don't use positionfloat when dealing with game map lists

	sf::Vector2f positionfloat;

	sf::Vector2f desiredpos;

	int IridiumCost;

	int KaskanCost;

	void CustomMove(double delta);

	void render(sf::RenderWindow* wind);

	void update(double delta);

	void turn(double delta);

};

class LandUnit : public Unit

{

public:

	using Unit::Unit;

	LandUnit();

	//usually attacks if on other unit, moves if not

	virtual void interact() = 0;



};

class BasicFighter : public LandUnit

{

public:

	using LandUnit::LandUnit;

	BasicFighter();

	int range;

	int RoF;

};

//BASIC FIGHTER, T1 

class LeopardTank : public BasicFighter

{

public:

	using BasicFighter::BasicFighter;

	void interact();

	LeopardTank(sf::Texture* tex, sf::Vector2f pos, map* gamemap);

};