
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
std::vector<UnitDependencies::TileDirection> Unit::pathCheck(double delta)
{
	std::vector<UnitDependencies::TileDirection> returner;
	//Simulating movement
	sf::Vector2f updiagnolleft(positionfloat.x - Speed*delta, positionfloat.y + Speed*delta);
	sf::Vector2f directionupdiagleft = normalize(updiagnolleft - positionfloat);
	sf::Vector2f MoveIntervalupdiagleft(directionupdiagleft.x*Speed*delta, directionupdiagleft.y*Speed*delta);
	//Definitions for updiagright
	sf::Vector2f updiagnolright(positionfloat.x + Speed*delta, positionfloat.y + Speed*delta);
	sf::Vector2f directionupdiagright = normalize(updiagnolright - positionfloat);
	sf::Vector2f MoveIntervalupdiagright(directionupdiagright.x*Speed*delta, directionupdiagright.y*Speed*delta);
	//End of updiagright definitions
	//Definitions for UP
	sf::Vector2f up(positionfloat.x, positionfloat.y + Speed*delta);
	sf::Vector2f directionup = normalize(up - positionfloat);
	sf::Vector2f MoveIntervalup(directionup.x*Speed*delta, directionup.y*Speed*delta);
	//End of UP definitions
	//Definitions for LEFT
	sf::Vector2f left(positionfloat.x - Speed*delta, positionfloat.y);
	sf::Vector2f directionleft = normalize(left - positionfloat);
	sf::Vector2f MoveIntervalleft(directionleft.x*Speed*delta, directionleft.y*Speed*delta);
	//End of LEFT definitions
	//Definitions for RIGHT
	sf::Vector2f right(positionfloat.x + Speed*delta, positionfloat.y);
	sf::Vector2f directionright = normalize(right - positionfloat);
	sf::Vector2f MoveIntervalright(directionright.x*Speed*delta, directionright.y*Speed*delta);
	//Definitions for Down
	//If statement checks if there are no obstructions. If there are no obstructions, this direction is added to the list of possible directions
	if (gamemap->tilegrid[std::round(positionfloat.x + MoveIntervalupdiagleft.x)][std::round(positionfloat.y + MoveIntervalupdiagleft.y)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x + MoveIntervalupdiagleft.x)][std::round(positionfloat.y + MoveIntervalupdiagleft.y)].type == availabletyles)
	{
		returner.push_back(UnitDependencies::UPDIAGNOLLEFT);
	}
	if (gamemap->tilegrid[std::round(positionfloat.x + MoveIntervalup.x)][std::round(positionfloat.y + MoveIntervalup.y)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x + MoveIntervalup.x)][std::round(positionfloat.y + MoveIntervalup.y)].type == availabletyles)
	{
		returner.push_back(UnitDependencies::UP);
	}
	if (gamemap->tilegrid[std::round(positionfloat.x + MoveIntervalupdiagright.x)][std::round(positionfloat.y + MoveIntervalupdiagright.y)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x + MoveIntervalupdiagright.x)][std::round(positionfloat.y + MoveIntervalupdiagright.y)].type == availabletyles)
	{
		returner.push_back(UnitDependencies::UPDIAGNOLRIGHT);
	}
	if (gamemap->tilegrid[std::round(positionfloat.x + MoveIntervalleft.x)][std::round(positionfloat.y + MoveIntervalleft.y)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x + MoveIntervalleft.x)][std::round(positionfloat.y + MoveIntervalleft.y)].type == availabletyles)
	{
		returner.push_back(UnitDependencies::LEFT);
	}
	if (gamemap->tilegrid[std::round(positionfloat.x + MoveIntervalright.x)][std::round(positionfloat.y + MoveIntervalright.y)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x + MoveIntervalright.x)][std::round(positionfloat.y + MoveIntervalright.y)].type == availabletyles)
	{
		returner.push_back(UnitDependencies::RIGHT);
	}
	
	//Stationary is in the logical order next(going left to right for every level, but it will be at the bottom due to its condition 
	return returner;
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
			gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = this;
			currstate = UnitDependencies::IDLE;
		}

		else
		{
			positionfloat += MoveInterval;
			//Sets unit on map
			gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = this;
			visual.setPosition(positionfloat.x * 64, positionfloat.y * 64);
			turn(delta);

		}

		//type changing

	}

}

void Unit::turn(double delta)

{

	//See physical paper: Explanation of turn appearance



	sf::Vector3f linedist(std::sqrt(std::pow(desiredpos.x - positionfloat.x, 2)), std::sqrt(std::pow(desiredpos.y - positionfloat.y, 2)), std::sqrt(std::pow(desiredpos.x - positionfloat.x, 2) + std::pow(desiredpos.y - positionfloat.y, 2)));

	double currentAngle = visual.getRotation();

	double targetAngle = (180 / 3.14)*(std::atan2(desiredpos.y - positionfloat.y, desiredpos.x - positionfloat.x));

	visual.setRotation(targetAngle + 90 * -1);

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

	visual.setPosition(positionfloat.x * 64, positionfloat.y * 64);

	//to obtain a certain unit size, simply devide the size by localbounds to get a ratio that you can put in scale

	visual.setScale(64 / visual.getLocalBounds().width, 64 / visual.getLocalBounds().height);

	visual.setColor(sf::Color::Red);

	gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = this;

	gamemap->mapunits.push_back(this);

}

void LeopardTank::interact()

{



}

