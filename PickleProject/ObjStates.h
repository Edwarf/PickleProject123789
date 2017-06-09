#pragma once
#include<SFML\Graphics.hpp>
#include"MapDefs.h"
#include"UnitDefs.h"
class cursor
{
public:
	sf::RenderWindow* wind;
	bool sleep = false;
	sf::Texture tex;
	cursor(std::string filepath, sf::RenderWindow* wind);
	sf::Sprite visual;
	cursor();
	void create(std::string, sf::RenderWindow* win);
	void update(double delta);
	void render(sf::RenderWindow* wind);
};
class MouseState
{
	cursor curs;
	map* gamemap;
	enum state
	{ 
		Idle,
		MakingSquare,
		GivingOrder
	};
public:
	sf::RenderWindow* wind;
	int speed = 60;
	void create(map*, sf::RenderWindow* wind);
	MouseState();
	std::vector<Unit*> TempUnitContainer;
	MouseState(map* gmap, sf::RenderWindow* wind);
	state currstate = Idle;
	bool moved = false;
	bool LastRightButtonDown = false;
	bool LastLeftButtonDown = false;
	bool RightButtonDown = false;
	bool LeftButtonDown = false;
	sf::Vector2f Position;
	sf::Vector2f initPosition;
	sf::Vector2f orderPosition;
	sf::RectangleShape selectRect;
	void update(double delta);
	void render(sf::RenderWindow* win);
};