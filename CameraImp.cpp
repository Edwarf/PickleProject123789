#include"stdafx.h"
#include"CameraDefs.h"
#include"UnitDefs.h"
#include<iostream>
Camera::Camera(sf::Vector2f vec, map* currmap, int w, int h, sf::RenderWindow* windd)
{
	wind = windd;
	currstate = Idle;
	position = vec;
	dispmap = currmap;
	width = camview.getSize().x / 64;
	height = camview.getSize().y / 64;
	wind->setView(camview);
}
void Camera::CustomMove(sf::Vector2f desiredpos, double delta)
{
	sf::Vector2f newpos(position);
	if (std::round(desiredpos.x) != std::round(position.x) || std::round(desiredpos.y) != std::round(position.y))
	{
		sf::Vector2f direction = normalize(desiredpos - position);
		sf::Vector2f MoveInterval(direction.x*speed*delta, direction.y*speed*delta);
		position += MoveInterval;
		camview.reset(sf::FloatRect(position.x*64, position.y*64, wind->getSize().x, wind->getSize().y));
		//type changing
	}
}
void Camera::create(sf::Vector2f vec, map* currmap, int w, int h, sf::RenderWindow* windd)
{
	wind = windd;
	position = vec;
	dispmap = currmap;
	currstate = Idle;
	width = camview.getSize().x / 64;
	height = camview.getSize().y / 64;
	wind->setView(camview);
}
void Camera::update(double delta)
{
	switch (currstate)
	{
		//In all scrolling cases, the coordinates for the custom move are the current position minus or plus an x or y coordinate. It wont get to the desired position, 
		//but it will approach based on speed.
	case Idle:
		break;
	case ScrollingUp:
		CustomMove(sf::Vector2f(position.x, position.y - 5), delta);
		break;
	case ScrollingDown:
		CustomMove(sf::Vector2f(position.x, position.y + 5), delta);
		break;
	case ScrollingLeft:
		CustomMove(sf::Vector2f(position.x - 5, position.y), delta);
		break;
	case ScrollingRight:
		CustomMove(sf::Vector2f(position.x + 5, position.y), delta);
		break;
	}
	if (position.x < 0)
	{
		position.x = 0;
	}
	if (position.y < 0)
	{
		position.y = 0;
	}
	width = camview.getSize().x/64;
	height = camview.getSize().y/64;

	//Loading local map from global map 

	//State will be manipulated by event manager
}
void Camera::render()
{
	wind->setView(camview);
	dispmap->renderpart(wind,  position.x,position.y, width + 2, height + 2);

}
Camera::Camera()
{
	currstate = Unininitialized;
}