#pragma once
#include<SFML\Graphics.hpp>
class map;
class UnitDependencies;
class TileDependencies;
class BuildingDependencies;
class GUIDependencies;
class map;
class Unit;
class Building;
class MouseState;
class ProjectileDependencies;
class Team
{
	sf::RenderWindow* win;
	void destroyMemory();
	//Initialized Values
	map* gamemap;
	UnitDependencies* unitdep;
	TileDependencies* tiledep;
	BuildingDependencies* buildingdep;
	MouseState* mouse;
	GUIDependencies* guidep; 
	ProjectileDependencies* projectdepend;
public:
	//Initialized Values
	double IridiumCapacity;
	double KaskanCapacity;
	//Team Number
	int number; 
	//Runtime determined values
	double totalIridium;
	double totalKaskan;
	std::vector<Unit*> TeamUnits;
	std::vector<Unit*> DeadUnits;
	std::vector<Building*> TeamBuildings;
	Team(UnitDependencies* unitdepC, TileDependencies* tiledepC, BuildingDependencies* buildingdepC, GUIDependencies* guidepC, ProjectileDependencies* projectdependC, map* gamemapC, int numberC,sf::RenderWindow* wind, MouseState* mouseC);
	void update(double delta);
	void render(sf::RenderWindow* wind);
};