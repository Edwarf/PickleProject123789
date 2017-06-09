#pragma once
#include<SFML\Graphics.hpp>
#include<vector>
class Unit;
class TileDependencies
{
public:
	enum tileType
	{
		Aquatic,
		Flatland,
		Elevated,
		ExtremeElevated
	};
	sf::Texture grasstex;
	TileDependencies();
};
class tile
{
public:
	Unit* currunit = nullptr;
	bool occupied;
	TileDependencies::tileType type;
	sf::Vector2f pos;
	sf::Sprite visual;
	tile(sf::Texture* tex, sf::Vector2f pos, TileDependencies::tileType type);
	void update(sf::Time delta);
	void render(sf::RenderWindow* wind);
};
class map
{
public:
	std::vector<Unit*> mapunits;
	//takes all live unit pointers from the given rectangle of coordinates
	std::vector<Unit*> retrieveUnits(int x, int y, int width, int height);
	TileDependencies Dependencies;
	std::vector<std::vector<tile>> tilegrid;
	map(int width, int height);
	map();
	void create(int x, int y);
	void renderpart(sf::RenderWindow* wind, int x, int y, int width, int height);
	void update(double delta);
	void render(sf::RenderWindow* wind);
};
