#pragma once
#include<SFML\Graphics.hpp>
#include<iostream>
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
		//Not a unit, used in production
		NULLUNITTYPE,
		LeopardTank,
	};
	static const int LeopardTankHP = 1000;
	static const int LeopardTankIridiumCost = 100;
	static const int LeopardTankKaskanCost = 100;
	static const int LeopardTankArmor = 1;
	static const int LeopardTankSpeed = 2;
	static const int LeopardTankRange = 1;
	static const int LeopardTankVisionRange = 3;
	static constexpr double LeopardTankRof = 0.5;
	static constexpr double LeopardTankDamage = 100;
	sf::Texture LeopardTex;
	UnitDependencies();
};
class BuildingDependencies
{
public:
	enum BuildingStates
	{
		IDLE,
		//Constructing state is exclusively for production buildings
		ACTING,

	};

};

class GUIDependencies
{
public:
	enum ElementState
	{
		IDLE,
		LEFTCLICKED,
		RIGHTCLICKED,
	};
	sf::Texture DefaultBackGroundGUITex;
	GUIDependencies();

};
class TileDependencies
{
public:
	enum TileState
	{
		IDLE,
		//In fog of war
		VISIBLE,
		REVEALED,
	};
	enum tileType
	{
		Aquatic,
		Flatland,
		Elevated,
		ExtremeElevated
	};
	sf::Texture grasstex;
	sf::Texture GoldenRocksTex;
	TileDependencies();
};
class TeamDependencies
{
public:
	enum TeamState
	{
		IDLE,
	};
	static constexpr double TeamIridiumCapacity = 1000;
	static constexpr double TeamKaskanCapacity = 1000;
};