#include"stdafx.h"
#include"ObjStates.h"
#include"UnitDefs.h"
#include<iostream>
ControlKey::ControlKey(sf::Keyboard::Key k, std::vector<Unit*> un)
{
	sig = k;
	associatedUnits = un;
}
cursor::cursor()
{

}
cursor::cursor(std::string filepath, sf::RenderWindow* win)
{
	wind = win;
	if (!tex.loadFromFile(filepath))
	{
		std::cout << "Error, could not load cursor image file";
	}
	else
	{
		visual.setTexture(tex);
	}
	visual.setScale(32 / visual.getLocalBounds().width, 32 / visual.getLocalBounds().height);
}
void cursor::create(std::string filepath, sf::RenderWindow* win)
{
	wind = win;
	if (!tex.loadFromFile(filepath))
	{
		std::cout << "Error, could not load cursor image file";
	}
	else
	{
		visual.setTexture(tex);
	}
	visual.setScale(32 / visual.getLocalBounds().width, 32 / visual.getLocalBounds().height);
}
//Cursors are updated in mousstate, not in the main loop
void cursor::update(double delta)
{
	if (!sleep)
		visual.setPosition(wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))));
	
}
void cursor::render(sf::RenderWindow* wind)
{
	wind->draw(visual);
}

void MouseState::update(double delta)
{
	//RightButtonDown manipulated externally, do NOT change its value unless refactoring. 
	if (LeftButtonDown == true && LastLeftButtonDown == false)
	{
		if (RenderingPhantomBuilding && phantbuild != nullptr && !phantbuild->colliding)
		{
			currstate = Idle;
			phantbuild->constBuilder->pushBackConstruction(delta);
			phantbuild = nullptr;
		}
		else
		{
			initPosition.x = wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).x;
			initPosition.y = wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).y;
			currstate = MakingSquare;
			LastLeftButtonDown = true;
			selectRect.setPosition(initPosition.x, initPosition.y);
			if (GUIUp)
			{
				for (int i = 0; i < currGU->values.size(); i++)
				{
					if (currGU->values[i]->visual.getGlobalBounds().contains(sf::Vector2f(initPosition.x, initPosition.y)))
					{
						LeftButtonDown = false;
						LastLeftButtonDown = false;
						currGU->values[i]->currstate = GUIDependencies::ElementState::LEFTCLICKED;
						currstate = LastGUIState;
					}
				}
			}
		}
	}
	//If the left mouse button goes up then this code is executed.
	else if (LeftButtonDown == false && LastLeftButtonDown == true)
	{
		LastLeftButtonDown = false;
		currstate = Idle;
		TempUnitContainer.clear();
		TempBuildingContainer.clear();
		if (Position == initPosition)
		{
			bool activated = false;
			std::vector<tile*> tiles = gamemap->returnMapSquare(Position.x/64 - 1, Position.y/64 - 1, 2, 2);
			tile* temptile = gamemap->returnTile(Position.x / 64, Position.y / 64);
			for (int i = 0; i < tiles.size(); i++)
			{
				if (tiles[i]->currunit != nullptr && tiles[i]->currunit->visual.getGlobalBounds().contains(Position))
				{
					currstate = RenderingLoneUnit;
					currGU = &temptile->currunit->UnitUI;
					activated = true;
					TempUnitContainer.push_back(tiles[i]->currunit);
					
					break;
				}
			}
			if (temptile != nullptr && !activated)
			{

				if (temptile->currbuilding != nullptr)
				{
					currGU = &temptile->currbuilding->UIBuilding;
					TempBuildingContainer.push_back(temptile->currbuilding);
					currstate = RenderingLoneBuilding;
				}
				else
				{
					currstate = Idle;
				}
			}
		}
		else
		{
			//THE IF/ELSEIFS BELOW REORIENT THE ORIGIN OF THE RECTANGLE TO THE TOP LEFT. DON'T EDIT(ITS FUCKING WEIRD) IF YOU DON'T WANT TO CHANGE RECT BEHAVIOR. 
			if (std::sqrt(std::pow((Position.x - initPosition.x), 2)) / (Position.x - initPosition.x) == -1)
			{
				if (std::sqrt(std::pow((Position.y - initPosition.y), 2)) / (Position.y - initPosition.y) == 1)
				{
					initPosition.x = initPosition.x + selectRect.getSize().x;
					selectRect.setSize(sf::Vector2f(selectRect.getSize().x*-1, selectRect.getSize().y));
				}
				else if (std::sqrt(std::pow((Position.y - initPosition.y), 2)) / (Position.y - initPosition.y) == -1)
				{
					initPosition.x = initPosition.x + selectRect.getSize().x;
					initPosition.y = initPosition.y + selectRect.getSize().y;
					selectRect.setSize(sf::Vector2f(selectRect.getSize().x*-1, selectRect.getSize().y*-1));
				}
			}
			else if (std::sqrt(std::pow((Position.x - initPosition.x), 2)) / (Position.x - initPosition.x) == 1)
			{
				if (std::sqrt(std::pow((Position.y - initPosition.y), 2)) / (Position.y - initPosition.y) == 1)
				{

				}
				else if (std::sqrt(std::pow((Position.y - initPosition.y), 2)) / (Position.y - initPosition.y) == -1)
				{
					initPosition.y = initPosition.y + selectRect.getSize().y;
					selectRect.setSize(sf::Vector2f(selectRect.getSize().x, selectRect.getSize().y*-1));
				}
			}
			TempUnitContainer = gamemap->retrieveUnits(std::round(initPosition.x / 64), std::round(initPosition.y / 64), std::round(selectRect.getSize().x / 64), std::round(selectRect.getSize().y / 64));
			TempBuildingContainer = gamemap->retrieveBuildings(std::round(initPosition.x / 64), std::round(initPosition.y / 64), std::round(selectRect.getSize().x / 64), std::round(selectRect.getSize().y / 64));
			if (TempBuildingContainer.size() == 1)
			{
				currstate = RenderingLoneBuilding;
			}
			else if (TempUnitContainer.size() == 1)
			{
				currstate = RenderingLoneUnit;
			}
			else
			{
				currstate = Idle;
			}
		}
		selectRect.setSize(sf::Vector2f(0, 0));
		curs.sleep = false;
	}
	else if (RightButtonDown == true)
	{
		RightButtonDown = false;
		LeftButtonDown = false;
		orderPosition.x = wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).x;
		orderPosition.y = wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).y;
		currstate = GivingOrder;
		if (GUIUp)
		{
			for (int i = 0; i < currGU->values.size(); i++)
			{
				if (currGU->values[i]->visual.getGlobalBounds().contains(sf::Vector2f(orderPosition.x, orderPosition.y)))
				{
					currGU->values[i]->currstate = GUIDependencies::ElementState::RIGHTCLICKED;
					currstate = LastGUIState;
				}
			}
		}
		bool activated = false;
		std::vector<tile*> tiles = gamemap->returnMapSquare(orderPosition.x / 64 - 1, orderPosition.y / 64 - 1, 2, 2);
		tile* temptile = gamemap->returnTile(orderPosition.x / 64, orderPosition.y / 64);
		for (int i = 0; i < tiles.size(); i++)
		{
			if (tiles[i]->currunit != nullptr && tiles[i]->currunit->visual.getGlobalBounds().contains(Position))
			{
				std::cout << TempUnitContainer.size() << std::endl;
				for (auto z : TempUnitContainer)
				{
						if (z->team != tiles[i]->currunit->team)
						{
							currstate = Idle;
							z->currstate = UnitDependencies::UnitStates::ATTACKING;
							activated = true;
							z->linkedUnit = tiles[i]->currunit;
							z->linkedBuilding = nullptr;
						}
					}
				}
			}
			if (temptile != nullptr && !activated)
			{
				if (temptile->currbuilding != nullptr)
				{
					for (auto z : TempUnitContainer)
					{
						if (z->team != temptile->currbuilding->team)
						{
							currstate = Idle;
							activated = true;
							z->currstate = UnitDependencies::UnitStates::ATTACKING;
							z->linkedBuilding = temptile->currbuilding;
							z->linkedUnit = nullptr;
						}
					}
				}
			}
	}
	sf::Vector2f MousePosScrolling;
	curs.update(delta);
	switch (currstate)
	{
	case MakingSquare:
		GUIUp = false;
		//Creates rectangle
		if (moved == true)
		{
			curs.sleep = true;
			//sf::Vector2f direction = normalize(Position - initPosition);
			//sf::Vector2f MoveInterval(direction.x*speed*delta, direction.y*speed*delta);
			selectRect.setSize(Position - initPosition);
			MousePosScrolling.x = initPosition.x + selectRect.getLocalBounds().width * std::sqrt(std::pow((Position.x - initPosition.x), 2)) / (Position.x - initPosition.x);
			MousePosScrolling.y = initPosition.y + selectRect.getLocalBounds().height * std::sqrt(std::pow((Position.y - initPosition.y), 2)) / (Position.y - initPosition.y);
			curs.visual.setPosition(MousePosScrolling);
			moved = false;
		}			
		//type changing
		break;
	case GivingOrder:
		GUIUp = false;
		//Unit Interaction
		//std::vector<Unit*> tempUnitCont = gamemap->retrieveUnits(orderPosition.x / 64, orderPosition.y / 64, 1, 1);
		//if (tempUnitCont.size() > 0)
		//{
		//	if (tempUnitCont[0] != nullptr)
		//	{
		//		//Fill with code for attacking or interacting with unit
		//	}
		//}
		//Movement
		divertIntervalX = 0;
		 if(TempUnitContainer.size() > 0);
		{	
			 //come back to later
			//this code simply moves
			for (int i = 0; i < TempUnitContainer.size(); i++)
			{
				//implicit conversion to float done here. This provides the point to which units will move
				TempUnitContainer[i]->desiredpos.x = (orderPosition.x / 64) + divertIntervalX;
				//This line make units switch into the "moving" state
				TempUnitContainer[i]->desiredpos.y = orderPosition.y / 64 + divertIntervalX;
				TempUnitContainer[i]->currstate = UnitDependencies::MOVING;
				divertIntervalX++;
			}
		}
		if (lastState == RenderingLoneUnit)
		{
			currstate = RenderingLoneUnit;
		}
		else
		{
			currstate = Idle;
		}
		lastState = GivingOrder;
		break;
		//If this is called, the currControlGroupKey has been reset from the outside 
	case CreatingControlGroup:
		for (int i = 0; i < controlgroups.size(); i++)
		{
			//If a control group with the sig provided has already been created, this will simply allocate the selected units to the previously created control group and exit the function
			if (controlgroups[i].sig == currControlGroupKey)
			{
				controlgroups[i].associatedUnits = TempUnitContainer;
				currstate = Idle;
				lastState = CreatingControlGroup;
				break;
			}
		}
		currstate = Idle;
		//If not, a control group will be created
		controlgroups.push_back(ControlKey(currControlGroupKey, TempUnitContainer));
		lastState = CreatingControlGroup;
		break;
	case SelectingControlGroup:
		lastState = SelectingControlGroup;
		for (int i = 0; i < controlgroups.size(); i++)
		{
			//If a control group with the sig provided has already been created, this will simply allocate the selected units to the previously created control group and exit the function
			if (controlgroups[i].sig == currControlGroupKey)
			{
				TempUnitContainer = controlgroups[i].associatedUnits;
				currstate = Idle;
				lastState = SelectingControlGroup;
				break;
			}
		}
		currstate = Idle;
		break;
	case RenderingLoneUnit:
		GUIUp = true;
		if (TempUnitContainer.size() == 1)
		{
			currGU = &TempUnitContainer[0]->UnitUI;
		}
		currGU->update(delta);
		lastState = RenderingLoneUnit;
		LastGUIState = RenderingLoneUnit;
		break;
	case RenderingLoneBuilding:
		GUIUp = true;
		if (TempBuildingContainer.size() == 1)
		{
			currGU = &TempBuildingContainer[0]->UIBuilding;
		}
		currGU->update(delta);
		lastState = RenderingLoneBuilding;
		LastGUIState = RenderingLoneBuilding;
		break;
	case RenderingPhantomBuilding:
		GUIUp = true;
		phantbuild->update(delta, gamemap, wind);
		lastState = RenderingPhantomBuilding;
		break;
	case Idle:
		lastState = Idle;
		break;
	}
	moved = false;
}
MouseState::MouseState(map* gmap, sf::RenderWindow* win)
{
	wind = win;
	gamemap = gmap;
	selectRect.setOutlineColor(sf::Color::Green);
	selectRect.setFillColor(sf::Color::Transparent);
	selectRect.setOutlineThickness(2.0);
	curs.create("Cursor.png", wind);
}
MouseState::MouseState()
{

}
void MouseState::create(map* gmap, sf::RenderWindow* win)
{
	wind = win;
	gamemap = gmap;
	curs.create("Cursor.png", wind);
	selectRect.setFillColor(sf::Color::Transparent);
	selectRect.setOutlineColor(sf::Color::Green);
	selectRect.setOutlineThickness(2.0);
}
void MouseState::render(sf::RenderWindow* win)
{
	if (currstate == RenderingLoneUnit || currstate == RenderingLoneBuilding)
	{
		currGU->render(win);
	}
	else if (currstate == RenderingPhantomBuilding)
	{
		phantbuild->render(win);
	}
	win->draw(selectRect);
	curs.render(win);
}