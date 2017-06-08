#pragma once
#include<SFML\Graphics.hpp>
#include"MapDefs.h"
sf::Vector2f normalize(const sf::Vector2f& source);
class UnitDependencies
{
public:
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
	//Number of pathposition at which to move to
	int pathPositionIterations = 0;
	void pathfind(double delta);
	UnitDependencies::UnitStates currstate;
	Unit();
	TileDependencies::tileType availabletyles;
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
	std::vector<sf::Vector2f> pathPositions;
	int IridiumCost;
	int KaskanCost;
	void CustomMove(double delta);
	bool CustomMovePathfinding(double delta, sf::Vector2f*startingPoint,sf::Vector2f* position);
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