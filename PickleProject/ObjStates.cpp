#include"stdafx.h"
#include"ObjStates.h"
#include"UnitDefs.h"
#include<iostream>
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
		LastLeftButtonDown = true;
		currstate = MakingSquare;
		initPosition.x = wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).x;
		initPosition.y = wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).y;
		//sets up initial rectangle position
		selectRect.setPosition(initPosition.x, initPosition.y);
	}
	//If the left mouse button goes up then this code is executed.
	else if (LeftButtonDown == false && LastLeftButtonDown == true)
	{
		LastLeftButtonDown = false;
		currstate = Idle;
		//THE IF/ELSEIF BELOW REORIENT THE ORIGIN OF THE RECTANGLE TO THE TOP LEFT. DON'T EDIT(ITS FUCKING WEIRD) IF YOU DON'T WANT TO CHANGE RECT BEHAVIOR. 

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
		selectRect.setSize(sf::Vector2f(0, 0));
		curs.sleep = false;
		//Gives all selected units to internal list

	}
	else if (RightButtonDown == true)
	{
		RightButtonDown = false;
		LeftButtonDown = false;
		currstate = GivingOrder;
		orderPosition.x = wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).x;
		orderPosition.y = wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).y;
	}
	sf::Vector2f MousePosScrolling;
	curs.update(delta);
	switch (currstate)
	{
	case MakingSquare:
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
		//Unit Interaction
		std::vector<Unit*> tempUnitCont = gamemap->retrieveUnits(orderPosition.x / 64, orderPosition.y / 64, 1, 1);
		if (tempUnitCont.size() > 0)
		{
			if (tempUnitCont[0] != nullptr)
			{
				//Fill with code for attacking or interacting with unit
			}
		}
		//Movement
		else if(TempUnitContainer.size() > 0);
		{		
			//this code simply moves
			for (int i = 0; i < TempUnitContainer.size(); i++)
			{
				//implicit conversion to float done here. This provides the point to which units will move
				TempUnitContainer[i]->desiredpos.x = orderPosition.x / 64;
				TempUnitContainer[i]->desiredpos.y = orderPosition.y / 64;
				//This line make units switch into the "moving" state
				TempUnitContainer[i]->currstate = UnitDependencies::MOVING;
			}
			TempUnitContainer.clear();
		}
		currstate = Idle;
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
	win->draw(selectRect);
	curs.render(win);
}