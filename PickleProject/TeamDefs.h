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
class Team
{
	sf::RenderWindow* win;
	void destroyMemory();
	//Initialized Values
	map* gamemap;
	UnitDependencies* unitdep;
	TileDependencies* tiledep;
	BuildingDependencies* buildingdep;
	GUIDependencies* guidep; 
public:
	//Initialized Values
	double IridiumCapacity = 1000;
	double KaskanCapacity = 1000;
	//Team Number
	int number; 
	//Runtime determined values
	double totalIridium;
	double totalKaskan;
	std::vector<Unit*> TeamUnits;
	std::vector<Building*> TeamBuildings;
	Team(UnitDependencies* unitdepC, TileDependencies* tiledepC, BuildingDependencies* buildingdepC, GUIDependencies* guidepC, map* gamemapC, int numberC, sf::RenderWindow* wind);
	void update(double delta);
	void render(sf::RenderWindow* wind);
};