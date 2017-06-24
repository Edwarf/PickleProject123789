#pragma once
#include<SFML\Graphics.hpp>
#include<iostream>
class Element;
class MouseState;
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
		CONSTRUCTING,
		REPOSITIONING,
		ATTACKING,
		DEAD,
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
		ModernCommander,
		ModernLeopardTank,
		ModernT1Engineer
	};
	static const int ModernLeopardTankHP = 1000;
	static const int ModernLeopardTankIridiumCost = 100;
	static const int ModernLeopardTankKaskanCost = 100;
	static const int ModernLeopardTankArmor = 1;
	static const int ModernLeopardTankSpeed = 2;
	static const int ModernLeopardTankRange = 5;
	static const int ModernLeopardTankVisionRange = 3;
	static constexpr double ModernLeopardTankRof = 0.5;
	static constexpr double ModernLeopardTankDamage = 100;
	static const AttackType ModernLeopardTankAttackType = Laser;
	sf::Texture LeopardTex;
	//////////////////////////////////////////////////////
	static constexpr double ModernT1EngineerHP = 500;
	static const int  ModernT1EngineerIridiumCost = 150;
	static const int ModernT1EngineerKaskanCost = 150;
	static const int ModernT1EngineerArmor = 0;
	static constexpr double ModernT1EngineerSpeed = 4;
	static const int  ModernT1EngineerVisionRange = 2;
	static const int  ModernT1EngineerRange = 3;
	static constexpr double  ModernT1EngineerProductionRate = 20;
	static const AttackType ModernT1EngineerAttackType = None;
	sf::Texture ModernT1EngineeerTex;
	///////////////////////////////////////////////////////
	static constexpr double ModernCommanderHP = 50000;
	static constexpr double ModernCommanderRof = 1.5;
	static constexpr double ModernCommanderDamage = 500;
	static constexpr double ModernCommanderProductionRate = 20;
	static const int ModernCommanderArmor = 75;
	static const int ModernCommanderRange = 8;
	static const int ModernCommanderSpeed = 1;
	static const int ModernCommanderVisionRange = 5;
	static const AttackType ModernCommanderAttackType = Laser;
	sf::Texture  ModernCommanderTex;
	/////////////////////////////////////////////////////
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
		DEAD,
	};
	enum BuildingType
	{
		NULLBUILDINGTYPE,
		ModernFactory,
	};
	static const int ModernFactoryIridiumCost = 1000;
	static const int ModernFactoryKaskanCost = 1000;
	static const int ModernFactoryTier = 1;
	static const int ModernFactoryTileHeight = 3;
	static const int ModernFactoryTileWidth = 3;
	static const int ModernFactoryVisionRange = 4;
	static constexpr double ModernFactoryProductionRate = 10;
	static constexpr double ModernFactoryHP = 5000;
	static constexpr double ModernFactoryArmor = 50;
	sf::Texture ModernFactoryTex;
	BuildingDependencies();
};

class GUIDependencies
{
	std::vector<std::vector<Element*>> GUIs;
public:
	enum ElementState
	{
		IDLE,
		LEFTCLICKED,
		RIGHTCLICKED,
	};
	sf::Texture ModernBackgroundBottomGUITex;
	GUIDependencies();
	//Modern Units
	std::vector<Element*> GetModernLeopardTankUI(sf::RenderWindow* wind, BuildingDependencies* buildingdepend);
	std::vector<Element*> GetModernT1EngineerUI(sf::RenderWindow* wind, BuildingDependencies* buildingdepend, MouseState* mouse);
	//Modern Buildings
	std::vector<Element*> GetModernFactoryUI(sf::RenderWindow* wind, UnitDependencies* unitdepend);
	//Memory Management
	void DeleteGUIs();
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
		ExtremeElevated,
		ANY
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
class ProjectileDependencies
{
public:
	enum ProjectileType 
	{
		LaserRound,
	};
	enum ProjectileState
	{
		IDLE,
		ONTARGET,
		MOVING,
		DEAD
	};
	ProjectileDependencies();
	sf::Texture StandardLaserRound;
	static const int StandardLaserRoundSpeed = 10;
};