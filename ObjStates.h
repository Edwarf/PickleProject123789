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
class ControlKey
{
public:
	//This variable is the signature of the control group. If a key is pressed and its signature is identical to this one, it will select units in the group
	sf::Keyboard::Key sig;
	std::vector<Unit*> associatedUnits;
	ControlKey(sf::Keyboard::Key k, std::vector<Unit*> un);
};
class MouseState
{
	cursor curs;
	map* gamemap;

public:
	enum state
	{
		Idle,
		MakingSquare,
		GivingOrder,
		ReadyingControlGroup,
		CreatingControlGroup,
		SelectingControlGroup,
	};
	std::vector<ControlKey> controlgroups;
	//this variable is for collecting potential control group keys. Initialized to unkown key.   
	sf::Keyboard::Key currControlGroupKey = sf::Keyboard::Key::Unknown;
	sf::RenderWindow* wind;
	int speed = 60;
	void create(map*, sf::RenderWindow* wind);
	MouseState();
	//TempUnitContainer is cleared whenever a new unit selection box is created
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