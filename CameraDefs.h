#pragma once
#include<SFML\Graphics.hpp>
#include"MapDefs.h"
class Camera
{
	
	map* dispmap;
	double speed = 20;
	sf::Vector2f position;
public:
	int width;
	int height;
	map localRender;
	sf::View camview;
	sf::RenderWindow* wind;
	enum state
	{
		Unininitialized,
		Idle,
		ScrollingUp,
		ScrollingDown,
		ScrollingLeft,
		ScrollingRight
	};
	Camera();
	state currstate;
	//w and h are the widths and heights for the display size
	Camera(sf::Vector2f startpos, map* currmap, int w, int h, sf::RenderWindow*wind);
	void CustomMove(sf::Vector2f desiredpos, double delta);
	void create(sf::Vector2f startpos, map* currmap, int w, int h, sf::RenderWindow* wind);
	void update(double delta);
	void render();
};