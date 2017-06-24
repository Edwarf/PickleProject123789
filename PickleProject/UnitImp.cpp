
#include"stdafx.h"
#include"UnitDefs.h"
#include<cmath>
#include<iostream> 
#include"ProjectileDefs.h"
#include<algorithm>
#include"BuildingDefs.h"
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
void Unit::update(double delta)
{
	if (HP <= 0)
	{
		currstate = UnitDependencies::UnitStates::DEAD;
	}
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
	case UnitDependencies::CONSTRUCTING:
		laststate = UnitDependencies::CONSTRUCTING; 
		performAction(delta);
		break;
	case UnitDependencies::ATTACKING:
		laststate = UnitDependencies::ATTACKING;
		performAction(delta);
		break;
	case UnitDependencies::DEAD:
		laststate = UnitDependencies::DEAD;
		break;
	case UnitDependencies::IDLE:
		break;
	}
	provideVision(delta);

}
void CombatUnit::update(double delta)
{
	if (HP <= 0)
	{
		currstate = UnitDependencies::UnitStates::DEAD;
	}
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
	case UnitDependencies::CONSTRUCTING:
		laststate = UnitDependencies::CONSTRUCTING;
		performAction(delta);
		break;
	case UnitDependencies::ATTACKING:
		laststate = UnitDependencies::ATTACKING;
		performAction(delta);
		break;
	case UnitDependencies::DEAD:
		for (int i = 0; i < projectileList.size(); i++)
		{
			delete projectileList[i];
		}
		projectileList.clear();
		laststate = UnitDependencies::DEAD;
		break;
	case UnitDependencies::IDLE:
		break;
	}
	for (int i = 0; i < projectileList.size(); i++)
	{
		projectileList[i]->update(delta);
		if (projectileList[i]->currstate == ProjectileDependencies::ProjectileState::DEAD)
		{
			delete projectileList[i];
			projectileList.erase(projectileList.begin() + i);
		}
	}
	provideVision(delta);
}
void Commander::update(double delta)
{
	if (HP <= 0)
	{
		currstate = UnitDependencies::UnitStates::DEAD;
	}
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
	case UnitDependencies::CONSTRUCTING:
		laststate = UnitDependencies::CONSTRUCTING;
		performAction(delta);
		break;
	case UnitDependencies::ATTACKING:
		laststate = UnitDependencies::ATTACKING;
		performAction(delta);
		break;
	case UnitDependencies::DEAD:
		for (int i = 0; i < projectileList.size(); i++)
		{
			delete projectileList[i];
		}
		projectileList.clear();
		laststate = UnitDependencies::DEAD;
		break;
	case UnitDependencies::IDLE:
		break;
	}
	for (int i = 0; i < projectileList.size(); i++)
	{
		projectileList[i]->update(delta);
		if (projectileList[i]->currstate == ProjectileDependencies::ProjectileState::DEAD)
		{
			delete projectileList[i];
			projectileList.erase(projectileList.begin() + i);
		}
	}
	provideVision(delta);
}
void Commander::render(sf::RenderWindow* win)
{
	win->draw(visual);
	for (int i = 0; i < projectileList.size(); i++)
	{
		projectileList[i]->render(win);
	}
}
void Unit::render(sf::RenderWindow* wind)
{
	wind->draw(visual);
}
void CombatUnit::render(sf::RenderWindow* win)
{
	win->draw(visual);
	for (int i = 0; i < projectileList.size(); i++)
	{
		projectileList[i]->render(win);
	}
}
void Unit::readjustDesiredPos(double delta)
{
	while (gamemap->tilegrid[std::round(desiredpos.x)][std::round(desiredpos.y)].currunit != nullptr ||  gamemap->tilegrid[std::round(desiredpos.x)][std::round(desiredpos.y)].currbuilding != nullptr || (gamemap->tilegrid[std::round(desiredpos.x)][std::round(desiredpos.y)].type != availabletyles && gamemap->tilegrid[std::round(desiredpos.x)][std::round(desiredpos.y)].type != availabletyles2))
	{
		desiredpos.x++;
	}
}
void Unit::realign(double delta)
{
	tile* tileinquestion;
	tileinquestion = gamemap->returnTile(positionfloat.x + 1, positionfloat.y + 1);
	tile* tileahead = gamemap->returnTile(positionfloat.x + 1, positionfloat.y);
	if (tileinquestion != nullptr)
	{
		if (tileinquestion->currunit != nullptr && tileahead != nullptr)
		{
			if (tileahead->currbuilding == nullptr && tileahead->currunit == nullptr)
			{
				desiredpos.x++;
				currstate = UnitDependencies::UnitStates::MOVING;
			}
		}
	}
	tileinquestion = gamemap->returnTile(positionfloat.x - 1, positionfloat.y - 1);
	tileahead = gamemap->returnTile(positionfloat.x, positionfloat.y - 1);
	if (tileinquestion != nullptr)
	{
		if (tileinquestion->currunit != nullptr && tileahead != nullptr)
		{
			if (tileahead->currbuilding == nullptr && tileahead->currunit == nullptr)
			{
				desiredpos.y--;
				currstate = UnitDependencies::UnitStates::MOVING;
			}
		}
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
void CombatUnit::attack(double delta)
{
	turn(delta);
	if (currRoF >= Rof)
	{
		switch (atktype)
		{
		case UnitDependencies::Laser:
			double lastROF = currRoF - Rof;
			currRoF = 0;
			currRoF += lastROF;
			projectileList.push_back(new Projectile(atktype, ProjectileDependencies::StandardLaserRoundSpeed, gamemap, damage, team, positionfloat, desiredpos, &projectdepend->StandardLaserRound, linkedUnit, TileDependencies::tileType::Flatland, ProjectileDependencies::ProjectileState::MOVING));
			break;
		}
	}
	else
	{
		currRoF += delta;
	}
}
void T1BasicFighter::performAction(double delta)
{
	if (linkedUnit != nullptr && linkedUnit->currstate == UnitDependencies::UnitStates::DEAD)
	{
		linkedUnit = nullptr;
	}
	if (linkedBuilding != nullptr && linkedBuilding->currstate == BuildingDependencies::BuildingStates::DEAD)
	{
		linkedBuilding = nullptr;
	}
	switch (currstate)
	{
	case UnitDependencies::UnitStates::ATTACKING:
		if (linkedUnit != nullptr)
		{
			desiredpos = linkedUnit->positionfloat;
		}
		else if (linkedBuilding != nullptr)
		{
			desiredpos = sf::Vector2f(linkedBuilding->location);
		}
		sf::FloatRect comparator(sf::Vector2f(desiredpos.x  - range, desiredpos.y  - range), sf::Vector2f(range * 2, range * 2));
		if (comparator.contains(positionfloat.x, positionfloat.y))
		{
			attack(delta);
		}
		else
		{
			CustomMove(delta);
		}
		break;
	}
}

//Put this function after definitions of faction and unittype. Ideally, it will be the last function called in the constructor 
void Unit::DefaultUISetup(GUIDependencies* gui, BuildingDependencies* builddepend, sf::RenderWindow* wind)
{
	//never use 0 for x or y positions
	//Identifies faction, then identifies units based on the faction.
	switch(faction)
	{
	case UnitDependencies::UnitFaction::Modern:
		switch (type)
		{
		case UnitDependencies::UnitType::ModernLeopardTank:
			UnitUI.values = gui->GetModernLeopardTankUI(wind, builddepend);
			break;
		case UnitDependencies::UnitType::ModernT1Engineer:
			UnitUI.values = gui->GetModernT1EngineerUI(wind, builddepend, mouse);
			break;
		case UnitDependencies::UnitType::ModernCommander:
			UnitUI.values = gui->GetModernT1EngineerUI(wind, builddepend, mouse);
		}
		break;
	}
	CustomUISetup();
}
void Engineer::CustomUISetup()
{
	for (int i = 0; i < UnitUI.values.size(); i++)
	{
		UnitUI.values[i]->linkedEngineer = this;
	}
}
void Unit::CustomUISetup()
{

}
T1BasicFighter::T1BasicFighter(UnitDependencies::UnitType typeC,
	UnitDependencies::UnitFaction factionC,
	TileDependencies::tileType availabletylesC,
	//Returns all available tile directions
	map* gamemapC,
	UnitDependencies::AttackType atktypeC,
	double HPC,
	int IridiumCostC,
	int KaskanCostC,
	double ArmorC,
	int SpeedC,
	sf::Vector2f currentPosition,
	sf::Texture* tex,	
	GUIDependencies* GUIDep,
	sf::RenderWindow* wind,
	int visionRadiusC,
	int rangeC,
	double RofC,
	double damageC,
	BuildingDependencies* builddepend,
	ProjectileDependencies* projectdependC,
	int teamC,
	MouseState* mouseC
	)
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
	currRoF = Rof;
	projectdepend = projectdependC;
	range = rangeC;
	damage = damageC;
	team = teamC;
	mouse = mouseC;
	HP = HPC;
	//to obtain a certain unit size, simply divide the size by localbounds to get a ratio that you can put in scale
	currstate = UnitDependencies::IDLE;
	laststate = currstate;
	visual.setTexture(*tex);
	visual.setPosition(positionfloat.x * 64, positionfloat.y * 64);
	visual.setScale(64 / visual.getLocalBounds().width, 64 / visual.getLocalBounds().height);
	visual.setColor(sf::Color(255,0,255,255));
	visual.setOrigin(32, 32);
	gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = this;
	DefaultUISetup(GUIDep, builddepend,wind);
}

T1Engineer::T1Engineer(UnitDependencies::UnitType typeC,
	UnitDependencies::UnitFaction factionC,
	TileDependencies::tileType availabletylesC,
	TileDependencies::tileType availabletyles2C,
	//Returns all available tile directions
	map* gamemapC,
	UnitDependencies::AttackType atktypeC,
	double HPC,
	int IridiumCostC,
	int KaskanCostC,
	double ArmorC,
	int SpeedC,
	sf::Vector2f currentPosition,
	sf::Texture* tex,
	GUIDependencies* GUIDepC,
	sf::RenderWindow* windC,
	const int visionRadiusC,
	const int rangeC,
	double productionSpeedC,
	int teamC,
	BuildingDependencies* builddependC,
	UnitDependencies* unitdependC,
	ProjectileDependencies* projectdependC,
	MouseState* mouseC
)
{
	range = rangeC;
	type = typeC;
	faction = factionC;
	availabletyles = availabletylesC;
	availabletyles2 = availabletyles2C;
	gamemap = gamemapC;
	atktype = atktypeC;
	Armor = ArmorC;
	Speed = SpeedC;
	positionfloat = currentPosition;
	visionSideSize = visionRadiusC;
	productionSpeed = productionSpeedC;
	origProductionSpeed = productionSpeed;
	team = teamC;
	mouse = mouseC;
	currBuildingType = BuildingDependencies::BuildingType::NULLBUILDINGTYPE;
	sufficientIridium = false;
	sufficientKaskan = false;
	currentlyProducedIridium = 0;
	currentlyProducedKaskan = 0;
	builddepend = builddependC;
	GUIDep = GUIDepC;
	HP = HPC;
	unitdepend = unitdependC;
	projectdepend = projectdependC;
	wind = windC;
	//to obtain a certain unit size, simply divide the size by localbounds to get a ratio that you can put in scale
	currstate = UnitDependencies::IDLE;
	laststate = currstate;
	visual.setTexture(*tex);
	visual.setPosition(positionfloat.x * 64, positionfloat.y * 64);
	visual.setScale(64 / visual.getLocalBounds().width, 64 / visual.getLocalBounds().height);
	visual.setColor(sf::Color::Red);
	visual.setOrigin(32, 32);
	gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = this;
	DefaultUISetup(GUIDep, builddepend, wind);
}

void T1Engineer::performAction(double delta)
{
	switch (currstate)
	{
	case UnitDependencies::UnitStates::CONSTRUCTING:
		bool canConstruct = false;
		if (sf::FloatRect(sf::Vector2f(currconstruction->currMapLocation.x - range, currconstruction->currMapLocation.y - range), sf::Vector2f(currconstruction->width, currconstruction->height)).contains(positionfloat.x, positionfloat.y))
		{
			canConstruct = true;
		}
		else
		{
			canConstruct = false;
			desiredpos = sf::Vector2f(currconstruction->currMapLocation.x - range, currconstruction->currMapLocation.y - range);
			CustomMove(delta);
		}
		if (canConstruct)
		{
			if (lastconstruction != currconstruction)
			{
				sufficientIridium = false;
				sufficientKaskan = false;
				currentlyProducedIridium = 0;
				currentlyProducedKaskan = 0;
			}
			lastconstruction = currconstruction;
			//If production is not equal to productionqueue[0], user has canceled production. In that case, reset all production stats and set the current production type to productionqueue[0]
			//Adjusting production rates. 
			if (gamemap->teams[team].totalKaskan < 0)
			{
				int absolutevaluetotkask = gamemap->teams[team].totalKaskan*-1;
				//this takes the difference of the kaskan capacity and kaskan deficit. It then divides it by the capacity, obtaining a rate at which we multiply the production rate.
				productionSpeed *= (gamemap->teams[team].KaskanCapacity - absolutevaluetotkask) / gamemap->teams[team].KaskanCapacity;
			}
			if (gamemap->teams[team].totalIridium < 0)
			{
				int absolutevaluetotirid = gamemap->teams[team].totalIridium*-1;
				//this takes the difference of the iridium capacity and iridium deficit. It then divides it by the capacity, obtaining a rate at which we multiply the production rate.
				productionSpeed *= (gamemap->teams[team].IridiumCapacity - absolutevaluetotirid) / gamemap->teams[team].IridiumCapacity;
			}
			double effectiveProductionRate = productionSpeed*delta;
			//Producing units
			switch (currBuildingType)
			{
			case BuildingDependencies::BuildingType::ModernFactory:
				if (currentlyProducedIridium < BuildingDependencies::ModernFactoryIridiumCost)
				{
					//adds iridium to the currently produced unit
					currentlyProducedIridium += effectiveProductionRate;
					//takes iridium away from team economy
					gamemap->teams[team].totalIridium -= effectiveProductionRate;
				}
				else
				{
					sufficientIridium = true;
				}
				if (currentlyProducedKaskan < BuildingDependencies::ModernFactoryKaskanCost)
				{
					currentlyProducedKaskan += effectiveProductionRate;
					gamemap->teams[team].totalKaskan -= effectiveProductionRate;
				}
				else
				{
					sufficientKaskan = true;
				}
				if (sufficientIridium && sufficientKaskan)
				{
					//Produce the building tank, place it outside of building. 
					gamemap->teams[team].TeamBuildings.push_back(new UnitProductionBuilding(team, UnitDependencies::UnitFaction::Modern, BuildingDependencies::ModernFactoryHP, BuildingDependencies::ModernFactoryArmor, BuildingDependencies::ModernFactoryIridiumCost,
						UnitDependencies::ModernLeopardTankKaskanCost, BuildingDependencies::ModernFactoryProductionRate, gamemap, BuildingDependencies::ModernFactoryTier, BuildingDependencies::ModernFactoryTileHeight, BuildingDependencies::ModernFactoryTileWidth,
						BuildingDependencies::ModernFactoryVisionRange, sf::Vector2i(currconstruction->currMapLocation.x, currconstruction->currMapLocation.y), &builddepend->ModernFactoryTex, GUIDep, unitdepend, builddepend, wind, GUIDep->GetModernFactoryUI(wind, unitdepend), BuildingDependencies::BuildingType::ModernFactory, mouse, projectdepend));
					//Reset values, with exception of production rate
					currentlyProducedIridium = 0;
					currentlyProducedKaskan = 0;
					sufficientIridium = false;
					sufficientKaskan = false;
					delete currconstruction;
					currstate = UnitDependencies::UnitStates::IDLE;
				}
				break;
			}
			productionSpeed = origProductionSpeed;
			//Here, we create a phantom building, which we use to move around the map
		}
		break;
	}
}
Commander::Commander(UnitDependencies::UnitType typeC,
	UnitDependencies::UnitFaction factionC,
	TileDependencies::tileType availabletylesC,
	TileDependencies::tileType availabletyles2C,
	//Returns all available tile directions
	map* gamemapC,
	UnitDependencies::AttackType atktypeC,
	double HPC,
	int IridiumCostC,
	int KaskanCostC,
	double ArmorC,
	int SpeedC,
	double RofC,
	sf::Vector2f currentPosition,
	sf::Texture* tex,
	GUIDependencies* GUIDepC,
	sf::RenderWindow* windC,
	const int visionRadiusC,
	const int rangeC,
	double productionSpeedC,
	int teamC,
	BuildingDependencies* builddependC,
	UnitDependencies* unitdependC,
	ProjectileDependencies* projectdependC,
	MouseState* mouseC,
	double damageC)
{
	range = rangeC;
	type = typeC;
	faction = factionC;
	availabletyles = availabletylesC;
	availabletyles2 = availabletyles2C;
	gamemap = gamemapC;
	atktype = atktypeC;
	Armor = ArmorC;
	Speed = SpeedC;
	positionfloat = currentPosition;
	visionSideSize = visionRadiusC;
	productionSpeed = productionSpeedC;
	origProductionSpeed = productionSpeed;
	team = teamC;
	mouse = mouseC;
	Rof = RofC;
	currRoF = Rof;
	currBuildingType = BuildingDependencies::BuildingType::NULLBUILDINGTYPE;
	sufficientIridium = false;
	sufficientKaskan = false;
	currentlyProducedIridium = 0;
	currentlyProducedKaskan = 0;
	builddepend = builddependC;
	GUIDep = GUIDepC;
	unitdepend = unitdependC;
	projectdepend = projectdependC;
	wind = windC;
	damage = damageC;
	HP = HPC;
	//to obtain a certain unit size, simply divide the size by localbounds to get a ratio that you can put in scale
	currstate = UnitDependencies::IDLE;
	laststate = currstate;
	visual.setTexture(*tex);
	visual.setPosition(positionfloat.x * 64, positionfloat.y * 64);
	visual.setScale(128 / visual.getLocalBounds().width, 128 / visual.getLocalBounds().height);
	visual.setOrigin(visual.getLocalBounds().width*visual.getScale().x/2, visual.getLocalBounds().height*visual.getScale().y/2);
	gamemap->tilegrid[std::round(positionfloat.x)][std::round(positionfloat.y)].currunit = this;
	DefaultUISetup(GUIDep, builddepend, wind);
}
void Commander::performAction(double delta)
{
	bool canConstruct = false;
	switch (currstate)
	{
	case UnitDependencies::UnitStates::CONSTRUCTING:
		if (sf::FloatRect(sf::Vector2f(currconstruction->currMapLocation.x - range, currconstruction->currMapLocation.y - range), sf::Vector2f(currconstruction->width, currconstruction->height)).contains(positionfloat.x, positionfloat.y))
		{
			canConstruct = true;
		}
		else
		{
			canConstruct = false;
			desiredpos = sf::Vector2f(currconstruction->currMapLocation.x - range, currconstruction->currMapLocation.y - range);
			CustomMove(delta);
		}
		if (canConstruct)
		{
			if (lastconstruction != currconstruction)
			{
				sufficientIridium = false;
				sufficientKaskan = false;
				currentlyProducedIridium = 0;
				currentlyProducedKaskan = 0;
			}
			lastconstruction = currconstruction;
			//If production is not equal to productionqueue[0], user has canceled production. In that case, reset all production stats and set the current production type to productionqueue[0]
			//Adjusting production rates. 
			if (gamemap->teams[team].totalKaskan < 0)
			{
				int absolutevaluetotkask = gamemap->teams[team].totalKaskan*-1;
				//this takes the difference of the kaskan capacity and kaskan deficit. It then divides it by the capacity, obtaining a rate at which we multiply the production rate.
				productionSpeed *= (gamemap->teams[team].KaskanCapacity - absolutevaluetotkask) / gamemap->teams[team].KaskanCapacity;
			}
			if (gamemap->teams[team].totalIridium < 0)
			{
				int absolutevaluetotirid = gamemap->teams[team].totalIridium*-1;
				//this takes the difference of the iridium capacity and iridium deficit. It then divides it by the capacity, obtaining a rate at which we multiply the production rate.
				productionSpeed *= (gamemap->teams[team].IridiumCapacity - absolutevaluetotirid) / gamemap->teams[team].IridiumCapacity;
			}
			double effectiveProductionRate = productionSpeed*delta;
			//Producing units
			switch (currBuildingType)
			{
			case BuildingDependencies::BuildingType::ModernFactory:
				if (currentlyProducedIridium < BuildingDependencies::ModernFactoryIridiumCost)
				{
					//adds iridium to the currently produced unit
					currentlyProducedIridium += effectiveProductionRate;
					//takes iridium away from team economy
					gamemap->teams[team].totalIridium -= effectiveProductionRate;
				}
				else
				{
					sufficientIridium = true;
				}
				if (currentlyProducedKaskan < BuildingDependencies::ModernFactoryKaskanCost)
				{
					currentlyProducedKaskan += effectiveProductionRate;
					gamemap->teams[team].totalKaskan -= effectiveProductionRate;
				}
				else
				{
					sufficientKaskan = true;
				}
				if (sufficientIridium && sufficientKaskan)
				{
					//Produce the building tank, place it outside of building. 
					gamemap->teams[team].TeamBuildings.push_back(new UnitProductionBuilding(team, UnitDependencies::UnitFaction::Modern, BuildingDependencies::ModernFactoryHP, BuildingDependencies::ModernFactoryArmor, BuildingDependencies::ModernFactoryIridiumCost,
						UnitDependencies::ModernLeopardTankKaskanCost, BuildingDependencies::ModernFactoryProductionRate, gamemap, BuildingDependencies::ModernFactoryTier, BuildingDependencies::ModernFactoryTileHeight, BuildingDependencies::ModernFactoryTileWidth,
						BuildingDependencies::ModernFactoryVisionRange, sf::Vector2i(currconstruction->currMapLocation.x, currconstruction->currMapLocation.y), &builddepend->ModernFactoryTex, GUIDep, unitdepend, builddepend, wind, GUIDep->GetModernFactoryUI(wind, unitdepend), BuildingDependencies::BuildingType::ModernFactory, mouse, projectdepend));
					//Reset values, with exception of production rate
					currentlyProducedIridium = 0;
					currentlyProducedKaskan = 0;
					sufficientIridium = false;
					sufficientKaskan = false;
					delete currconstruction;
					currstate = UnitDependencies::UnitStates::IDLE;
				}
				break;
			}
			productionSpeed = origProductionSpeed;
		}
		break;
	case UnitDependencies::UnitStates::ATTACKING:
		if (linkedUnit != nullptr && linkedUnit->currstate == UnitDependencies::UnitStates::DEAD)
		{
			linkedUnit = nullptr;
		}
		if (linkedBuilding != nullptr && linkedBuilding->currstate == BuildingDependencies::BuildingStates::DEAD)
		{
			linkedBuilding = nullptr;
		}
		switch (currstate)
		{
		case UnitDependencies::UnitStates::ATTACKING:
			if (linkedUnit != nullptr && linkedUnit->currstate != UnitDependencies::UnitStates::DEAD)
			{
				desiredpos = linkedUnit->positionfloat;
			}
			else if (linkedBuilding != nullptr && linkedBuilding->currstate != BuildingDependencies::BuildingStates::DEAD)
			{
				sf::Vector2i idealPos = linkedBuilding->location;
				sf::Vector2f differenceStorage(std::sqrt(std::pow(positionfloat.x - linkedBuilding->location.x, 2)), std::sqrt(std::pow(positionfloat.y - linkedBuilding->location.y, 2)));
				for (int i = linkedBuilding->location.x; i < linkedBuilding->location.x + linkedBuilding->tilewidth; i+= linkedBuilding->tilewidth - 1)
				{
					double comparatorx = std::sqrt(std::pow(positionfloat.x - i, 2));
					if (comparatorx < differenceStorage.x)
					{
						idealPos.x = i;
						differenceStorage.x = comparatorx;
					}
					for (int z = linkedBuilding->location.y; z < linkedBuilding->location.y + linkedBuilding->tileheight; z++)
					{
						double comparatory = std::sqrt(std::pow(positionfloat.y - z, 2));
						if (comparatory <  differenceStorage.y)
						{
							std::cout << "LULEDA";
							idealPos.y = z;
							differenceStorage.y = comparatory;
						}
					}

				}
				for (int i = linkedBuilding->location.y; i < linkedBuilding->location.y + linkedBuilding->tileheight; i += linkedBuilding->tileheight - 1)
				{
					double comparatory = std::sqrt(std::pow(positionfloat.x - i, 2));
					if (comparatory < differenceStorage.x)
					{
						idealPos.x = i;
						differenceStorage.x = comparatory;
					}
					for (int z = linkedBuilding->location.x; z < linkedBuilding->location.x + linkedBuilding->tilewidth; z++)
					{
						double comparatorx = std::sqrt(std::pow(positionfloat.x - z, 2));
						if (comparatorx <  differenceStorage.y)
						{
							idealPos.x = z;
							differenceStorage.x = comparatorx;
						}
					}

				}
				desiredpos = sf::Vector2f(idealPos);
			}
			sf::FloatRect comparator(sf::Vector2f(desiredpos.x - range, desiredpos.y - range), sf::Vector2f(range * 2, range * 2));
			if (comparator.contains(positionfloat.x, positionfloat.y))
			{
				attack(delta);
			}
			else
			{
				CustomMove(delta);
			}
			break;
		}
		break;
	}
}
void Unit::Dispose()
{
	gamemap->returnTile(std::round(positionfloat.x), std::round(positionfloat.y))->currunit = nullptr;
}