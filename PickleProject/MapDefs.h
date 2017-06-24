#pragma once
#include<SFML\Graphics.hpp>
#include"ObjectDependencyDefs.h"
#include<vector>
#include<set>
#include"TeamDefs.h"
class Building;
class Unit;

class tile
{
public:
	//color we reset to when rendering. 
	sf::Color resetColor;
	sf::Color origColor;
	TileDependencies::TileState currstate = TileDependencies::TileState::IDLE;
	std::set<int> currTeamsSeeing;
	Building* currbuilding = nullptr;
	Unit* currunit = nullptr;
	bool occupied;
	TileDependencies::tileType type;
	sf::Vector2f pos;
	sf::Sprite visual;
	tile(sf::Texture* tex, sf::Vector2f pos, TileDependencies::tileType type);
	void update(double );
	void render(sf::RenderWindow* wind);
};
class map
{
	MouseState* mouse;
	UnitDependencies* unitdep;
	TileDependencies* tiledep;
	BuildingDependencies* buildingdep;
	GUIDependencies* guidep;
	ProjectileDependencies* projectdepend;
public:
	tile* returnTile(int x, int y);
	std::vector<Team> teams;
	//This vector contains all tiles to be updated on the call. It is populated by the following functions: returnMapSquare
	std::set<tile*> tileUpdates;
	std::vector<tile*> returnMapSquare(int x, int y, int width, int height);
	std::vector<Unit> mapunits;
	//takes all live unit pointers from the given rectangle of coordinates
	std::vector<Unit*> retrieveUnits(int x, int y, int width, int height);
	std::vector<Building*> retrieveBuildings(int x, int y, int width, int height);
	TileDependencies Dependencies;
	std::vector<std::vector<tile>> tilegrid;
	map(int width, int height, int numberOfTeams, UnitDependencies* unitdepC, TileDependencies* tiledepC, BuildingDependencies* buildingdepC, GUIDependencies* guidepC, ProjectileDependencies* projectdependC,sf::RenderWindow* win, MouseState* mouseC);
	map();
	void create(int x, int y, int numberOfTeams, UnitDependencies* unitdepC, TileDependencies* tiledepC, BuildingDependencies* buildingdepC, GUIDependencies* guidepC, ProjectileDependencies* projectdependC, sf::RenderWindow* win, MouseState* mouseC);
	void renderpart(sf::RenderWindow* wind, int x, int y, int width, int height);
	void update(double delta);
	void render(sf::RenderWindow* wind);

};
