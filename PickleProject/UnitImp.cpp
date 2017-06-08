#include"stdafx.h"
#include"UnitDefs.h"
#include<cmath>
#include<iostream> 
#include"MapDefs.h"
UnitDependencies::UnitDependencies()
{
	if (!LeopardTex.loadFromFile(("LeopardTank.png")));
	{
		std::cout << "Error, could not load image.";
	}
}
sf::Vector2f normalize(const sf::Vector2f& source)
{
	//Square root of the squares of x and y to obtain length
	float length = std::sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		//Divides the length of the vector to source to normalize
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}
Unit::Unit()
{

}
void Unit::update(double delta)
{
	switch (currstate)
	{
	case UnitDependencies::MOVING:
		if (std::round(desiredpos.x) != std::round(positionfloat.x) || std::round(desiredpos.y) != std::round(positionfloat.y))
		{
			CustomMove(delta);
		}
		else
		{
			currstate = UnitDependencies::IDLE;
		}
		break;
	}
}
void Unit::render(sf::RenderWindow* wind)
{
	wind->draw(visual);
}
void Unit::CustomMove(double delta)
{
	sf::Vector2f newpos(positionfloat);
	if (std::round(desiredpos.x) != std::round(positionfloat.x) || std::round(desiredpos.y) != std::round(positionfloat.y))
	{
		//clears the previous tile
		gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = nullptr;
		//Normalizes the difference between vectors
		sf::Vector2f direction = normalize(desiredpos - positionfloat);
		sf::Vector2f MoveInterval(direction.x*Speed*delta, direction.y*Speed*delta);
		//Checks if the tile unit will enter is occupied
		if (gamemap->tilegrid[std::round(positionfloat.x + MoveInterval.x)][std::round(positionfloat.y + MoveInterval.y)].currunit != nullptr || gamemap->tilegrid[std::round(positionfloat.x + MoveInterval.x)][std::round(positionfloat.y + MoveInterval.y)].type != availabletyles)
		{
			currstate = UnitDependencies::IDLE;
		}
		else
		{
			positionfloat += MoveInterval;
			//Sets unit on map
			//gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = this;
			visual.setPosition(positionfloat.x * 64, positionfloat.y * 64);
			turn(delta);
		}
		//type changing
	}
}
void Unit::turn(double delta)
{
	//See physical paper: Explanation of turn appearance

	sf::Vector3f linedist (std::sqrt(std::pow(desiredpos.x - positionfloat.x, 2)), std::sqrt(std::pow(desiredpos.y - positionfloat.y, 2)), std::sqrt(std::pow(desiredpos.x - positionfloat.x, 2) + std::pow(desiredpos.y - positionfloat.y, 2)));
	double currentAngle = visual.getRotation();
	double targetAngle = (180/3.14)*(std::atan2(desiredpos.y - positionfloat.y , desiredpos.x - positionfloat.x));
	visual.setRotation(targetAngle+90*-1);
}
LandUnit::LandUnit()
{

}
BasicFighter::BasicFighter()
{

}
LeopardTank::LeopardTank(sf::Texture* tex, sf::Vector2f poss, map* currmaps)
{
	visual.setOrigin(32, 32);
	gamemap = currmaps;
	availabletyles = TileDependencies::Flatland;
	atktype = Physical;
	HP = 500;
	Armor = 1;
	//60 speed = 1 second of real time 
	Speed = 1;
	IridiumCost = 100;
	KaskanCost = 100;
	range = 4; 
	//Half a second per attack
	RoF = .5;
	positionfloat = poss;
	visual.setTexture((*tex));
	visual.setPosition(positionfloat.x*64, positionfloat.y*64);
	//to obtain a certain unit size, simply devide the size by localbounds to get a ratio that you can put in scale
	visual.setScale(64 / visual.getLocalBounds().width, 64 / visual.getLocalBounds().height);
	visual.setColor(sf::Color::Red);
	gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = this;
	gamemap->mapunits.push_back(this);
}
void LeopardTank::interact()
{
	
}

