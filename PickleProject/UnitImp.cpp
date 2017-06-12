
#include"stdafx.h"

#include"UnitDefs.h"

#include<cmath>

#include<iostream> 

#include<algorithm>

#include"MapDefs.h"
#include"GUIDefs.h"

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
		laststate = UnitDependencies::MOVING;
		if (std::round(desiredpos.x) != std::round(positionfloat.x) || std::round(desiredpos.y) != std::round(positionfloat.y))
		{
			CustomMove(delta);
		}
		else
		{
			currstate = UnitDependencies::IDLE;
			gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = this;
		}
		break;
		//desiredpos should be changed from OUTSIDE this case. We reset desiredpos to last_desired_pos, which should have been set OUTSIDE of this case by the object attempting to change UnitDependency's state.
	case UnitDependencies::REPOSITIONING:
		if (std::round(desiredpos.x) != std::round(positionfloat.x) || std::round(desiredpos.y) != std::round(positionfloat.y))
		{
			CustomMove(delta);
		}
		else
		{
			laststate = UnitDependencies::REPOSITIONING;
			currstate = laststate;
			desiredpos = last_desired_pos;
			last_desired_pos = desiredpos;
		}
		break;
	case UnitDependencies::ATTACKING:
		laststate = UnitDependencies::ATTACKING;
		performAction(delta);
		break;
	}
	provideVision(delta);

}

void Unit::render(sf::RenderWindow* wind)

{

	wind->draw(visual);

}
void Unit::readjustDesiredPos(double delta)
{
	while (gamemap->tilegrid[std::round(desiredpos.x)][std::round(desiredpos.y)].currunit != nullptr ||  gamemap->tilegrid[std::round(desiredpos.x)][std::round(desiredpos.y)].currbuilding != nullptr || (gamemap->tilegrid[std::round(desiredpos.x)][std::round(desiredpos.y)].type != availabletyles && gamemap->tilegrid[std::round(desiredpos.x)][std::round(desiredpos.y)].type != availabletyles2))
	{
		desiredpos.x++;
	}
}
//This function evaluates the move intervals(x and y) for every possible tile direction. It is then sorted based on the difference between them and the ideal move position in custommove
std::vector<sf::Vector2f> Unit::pathCheck(double delta)
{
	//Value for checking units
	int predictVal = 1;
	std::vector<sf::Vector2f> returner;
	//Simulating movement
	sf::Vector2f updiagnolleft(positionfloat.x - 5, positionfloat.y + 5);
	sf::Vector2f directionupdiagleft = normalize(updiagnolleft - positionfloat);
	sf::Vector2f MoveIntervalupdiagleft(directionupdiagleft.x*Speed*delta, directionupdiagleft.y*Speed*delta);
	//Definitions for updiagright
	sf::Vector2f updiagnolright(positionfloat.x + 5, positionfloat.y + 5);
	sf::Vector2f directionupdiagright = normalize(updiagnolright - positionfloat);
	sf::Vector2f MoveIntervalupdiagright(directionupdiagright.x*Speed*delta, directionupdiagright.y*Speed*delta);
	//End of updiagright definitions
	//Definitions for UP
	sf::Vector2f up(positionfloat.x, positionfloat.y + 5);
	sf::Vector2f directionup = normalize(up - positionfloat);
	sf::Vector2f MoveIntervalup(directionup.x*Speed*delta, directionup.y*Speed*delta);
	//End of UP definitions
	//Definitions for LEFT
	sf::Vector2f left(positionfloat.x - 5, positionfloat.y);
	sf::Vector2f directionleft = normalize(left - positionfloat);
	sf::Vector2f MoveIntervalleft(directionleft.x*Speed*delta, directionleft.y*Speed*delta);
	//End of LEFT definitions
	//Definitions for RIGHT
	sf::Vector2f right(positionfloat.x + 5, positionfloat.y);
	sf::Vector2f directionright = normalize(right - positionfloat);
	sf::Vector2f MoveIntervalright(directionright.x*Speed*delta, directionright.y*Speed*delta);
	//Definitions for DOWN
	sf::Vector2f down(positionfloat.x, positionfloat.y - 5);
	sf::Vector2f directiondown = normalize(down - positionfloat);
	sf::Vector2f MoveIntervaldown(directiondown.x*Speed*delta, directiondown.y*Speed*delta);
	//End of DOWN definitions
	//Defninitions for downdiagnolleft
	sf::Vector2f downdiagnolleft(positionfloat.x - 5, positionfloat.y - 5);
	sf::Vector2f directiondowndiagleft = normalize(downdiagnolleft - positionfloat);
	sf::Vector2f MoveIntervaldowndiagleft(directiondowndiagleft.x*Speed*delta, directiondowndiagleft.y*Speed*delta);
	//End of downdiagnolright definitions
	sf::Vector2f downdiagnolright(positionfloat.x + 5, positionfloat.y - 5);
	sf::Vector2f directiondowndiagright = normalize(downdiagnolright - positionfloat);
	sf::Vector2f MoveIntervaldowndiagright(directiondowndiagright.x*Speed*delta, directiondowndiagright.y*Speed*delta);
	//If statement checks if there are no obstructions. If there are no obstructions, this direction is added to the list of possible directions
	if (gamemap->tilegrid[std::round(positionfloat.x - predictVal)][std::round(positionfloat.y + predictVal)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x - predictVal)][std::round(positionfloat.y +predictVal)].type == availabletyles)
	{
		returner.push_back(MoveIntervalupdiagleft);
	}
	if (gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y + predictVal )].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y + predictVal)].type == availabletyles)
	{
		returner.push_back(MoveIntervalup);
	}
	if (gamemap->tilegrid[std::round(positionfloat.x  + predictVal)][std::round(positionfloat.y + predictVal)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x + predictVal)][std::round(positionfloat.y + predictVal)].type == availabletyles)
	{
		returner.push_back(MoveIntervalupdiagright);
	}
	if (gamemap->tilegrid[std::round(positionfloat.x - predictVal)][std::round(positionfloat.y)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x - predictVal)][std::round(positionfloat.y)].type == availabletyles)
	{
		returner.push_back(MoveIntervalleft);
	}
	if (gamemap->tilegrid[std::round(positionfloat.x + predictVal)][std::round(positionfloat.y)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x + predictVal)][std::round(positionfloat.y)].type == availabletyles)
	{
		returner.push_back(MoveIntervalright);
	}
	if (gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y - predictVal)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y - predictVal)].type == availabletyles)
	{
		returner.push_back(MoveIntervaldown);
	}
	if (gamemap->tilegrid[std::round(positionfloat.x - predictVal)][std::round(positionfloat.y - predictVal)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x - predictVal)][std::round(positionfloat.y - predictVal)].type == availabletyles)
	{
		returner.push_back(MoveIntervaldowndiagleft);
	}
	if (gamemap->tilegrid[std::round(positionfloat.x + predictVal)][std::round(positionfloat.y - predictVal)].currunit == nullptr && gamemap->tilegrid[std::round(positionfloat.x + predictVal)][std::round(positionfloat.y - 1)].type == availabletyles)
	{
		returner.push_back(MoveIntervaldowndiagright);
	}
	//Stationary is in the logical order next(going left to right for every level, but it will be at the bottom due to its condition 
	return returner;
}
void Unit::CustomMove(double delta)
{
	readjustDesiredPos(delta);
	//void the previous tile
	gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = nullptr;
	//Normalizes the difference between vectors
	sf::Vector2f direction = normalize(desiredpos - positionfloat);
	sf::Vector2f MoveInterval(direction.x*Speed*delta, direction.y*Speed*delta);
	positionfloat += MoveInterval;
	//Sets unit on map
	if (gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currbuilding != nullptr)
	{
		//provisional fix
		positionfloat -= MoveInterval;
	}
	else if (gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].type != availabletyles && gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].type != availabletyles2)
	{
		//provisional fix
		positionfloat -= MoveInterval;
	}
	else if (gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit != nullptr)
	{
		gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit->currstate = UnitDependencies::REPOSITIONING;
		gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit->last_desired_pos = gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit->desiredpos;
		gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit->desiredpos = sf::Vector2f((positionfloat.x + (1 * ((std::sqrt(std::pow(MoveInterval.x, 2)) / MoveInterval.x)*-1))), (positionfloat.y + (1 * ((std::sqrt(std::pow(MoveInterval.y, 2)) / MoveInterval.y)*-1))));
	}
	gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = this;
	//Sets the position of the visual sprite
	visual.setPosition(positionfloat.x * 64, positionfloat.y * 64);
	//Sets the position of the vision circle
	//Turns unit
	turn(delta);
	//Provides Vision
}
void Unit::provideVision(double delta)
{
	std::vector<tile*> visiontiles = gamemap->returnMapSquare(positionfloat.x - visionSideSize / 2, positionfloat.y - visionSideSize / 2, visionSideSize, visionSideSize);
	for (int i = 0; i < visiontiles.size(); i++)
	{
		visiontiles[i]->currstate = TileDependencies::TileState::VISIBLE;
		visiontiles[i]->currTeamsSeeing.insert(team);
	}
}

void Unit::turn(double delta)

{

	sf::Vector3f linedist(std::sqrt(std::pow(desiredpos.x - positionfloat.x, 2)), std::sqrt(std::pow(desiredpos.y - positionfloat.y, 2)), std::sqrt(std::pow(desiredpos.x - positionfloat.x, 2) + std::pow(desiredpos.y - positionfloat.y, 2)));
	double currentAngle = visual.getRotation();
	double targetAngle = ((180 / 3.14)*(std::atan2(desiredpos.y - positionfloat.y, desiredpos.x - positionfloat.x))) - 90;
	double realAngle = targetAngle - currentAngle;
	visual.rotate(realAngle);

	/*

	if (targetAngle > 0)
	{
		targetAngle = ((180 / 3.14)*(std::atan2(desiredpos.y - positionfloat.y, (desiredpos.x - positionfloat.x)*-1))) - 90;
		realAngle = (((targetAngle*-1 - currentAngle)*delta * 5));
		targetAngle = ((180 / 3.14)*(std::atan2(desiredpos.y - positionfloat.y, desiredpos.x - positionfloat.x))) - 90;
	}
	if (targetAngle < -90) 
	{
		targetAngle = ((180 / 3.14)*(std::atan2(desiredpos.y - positionfloat.y, (desiredpos.x - positionfloat.x)*-1))) - 90;
		realAngle = (((targetAngle*-1 - currentAngle)*delta * 5));
		targetAngle = ((180 / 3.14)*(std::atan2(desiredpos.y - positionfloat.y, desiredpos.x - positionfloat.x))) - 90;
	}
	if ((targetAngle < 0) && (targetAngle > -90))
	{
		targetAngle = ((-180 / 3.14)*(std::atan2((desiredpos.y - positionfloat.y), (desiredpos.x - positionfloat.x)))) - 270;
		realAngle = (((targetAngle*-1 - currentAngle)*delta * 5));
		targetAngle = ((180 / 3.14)*(std::atan2(desiredpos.y - positionfloat.y, desiredpos.x - positionfloat.x))) - 90;
	}
	double checkingAngle = (visual.getRotation()) - (targetAngle + 360);
	std::cout << realAngle << std::endl;
	if (checkingAngle  )
	{
	}
	visual.rotate(realAngle);
	double normalAngle = visual.getRotation();
	*/

}
void T1BasicFighter::performAction(double delta)
{
	exit(0);
}
LandUnit::LandUnit()

{



}
void Unit::DefaultUISetup(GUIDependencies* gui, sf::RenderWindow* wind)
{
	//never use 0 for x or y positions
	UnitUI.values.push_back(new BackgroundElement(0,780, 1920,200, &gui->DefaultBackGroundGUITex, wind));
}
T1BasicFighter::T1BasicFighter(UnitDependencies::UnitType typeC,
	UnitDependencies::UnitFaction factionC,
	TileDependencies::tileType availabletylesC,
	//Returns all available tile directions
	map* gamemapC,
	UnitDependencies::AttackType atktypeC,
	int HPC,
	int IridiumCostC,
	int KaskanCostC,
	int ArmorC,
	int SpeedC,
	sf::Vector2f currentPosition,
	sf::Texture* tex,
	GUIDependencies* GUIDep,
	sf::RenderWindow* wind,
	int visionRadiusC,
	int rangeC,
	double RofC,
	double damageC)
{
	type = typeC;
	faction = factionC;
	availabletyles = availabletylesC;
	availabletyles2 = availabletyles;
	gamemap = gamemapC;
	atktype = atktypeC;
	Armor = ArmorC;
	Speed = SpeedC;
	positionfloat = currentPosition;
	visionSideSize = visionRadiusC;
	Rof = RofC;
	range = rangeC;
	damage = damageC;

	//to obtain a certain unit size, simply divide the size by localbounds to get a ratio that you can put in scale
	currstate = UnitDependencies::IDLE;
	laststate = currstate;
	visual.setTexture(*tex);
	visual.setPosition(positionfloat.x * 64, positionfloat.y * 64);
	visual.setScale(64 / visual.getLocalBounds().width, 64 / visual.getLocalBounds().height);
	visual.setColor(sf::Color::Red);
	visual.setOrigin(32, 32);
	gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = this;
	DefaultUISetup(GUIDep, wind);
}

