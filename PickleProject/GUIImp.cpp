#include"stdafx.h"
#include"GUIDefs.h"
#include"UnitDefs.h"
#include"BuildingDefs.h"
#include<iostream>
#include"ObjStates.h"
ConstructorUnitElement::ConstructorUnitElement(double XC, double YC, double WIDTHC, double HEIGHTC, UnitDependencies::UnitType TYPEC, sf::Texture* tex, sf::RenderWindow* wind)
{
	this->wind = wind;
	x = XC;
	y = YC;
	type = TYPEC;
	width = WIDTHC;
	height = HEIGHTC;
	visual.setTexture(*tex);
	//Multiplies by this screen ratio and the ideal screen ratio
	x *= (wind->getView().getSize().x / 1940);
	y *= (wind->getView().getSize().y / 1080);
	//sets the position of the visual, multiplying the percentages XC and YC by the size of the view
	visual.setPosition((wind->getView().getCenter().x - wind->getView().getSize().x / 2) + x, (wind->getView().getCenter().y - wind->getView().getSize().y / 2) + y);
	//Multiplied width by a ratio of the current screen size and the screen size this game was made for. By doing this, we set width and height GUI values to a value that multiple systems can use. We only have to code for 1940/1080, however
	width *= (wind->getView().getSize().x / 1940);
	//Same operation with width as with height
	height *= (wind->getView().getSize().y / 1080);
	//Sets visuals scale to width and height divided by the current bounds of visual. This will result in visual's bounds being identical to width and height when multiplied by its scale
	visual.setScale(width / visual.getLocalBounds().width, height / visual.getLocalBounds().height);
}
void ConstructorUnitElement::update(double delta)
{
	visual.setPosition((wind->getView().getCenter().x - wind->getView().getSize().x / 2) + x, (wind->getView().getCenter().y - wind->getView().getSize().y / 2) + y);

	int prodsize = linkedProdBuilding->productionQueue.size();
	switch (currstate)
	{
	case GUIDependencies::ElementState::LEFTCLICKED:
		linkedProdBuilding->productionQueue.push_back(type);
		unitPositionsQueued.push_back(prodsize - 1);
		currstate = GUIDependencies::ElementState::IDLE;
		break;
	case GUIDependencies::ElementState::RIGHTCLICKED:
		for (int i = 0; i < unitPositionsQueued.size(); i++)
		{
			if (unitPositionsQueued[i] >= prodsize)
			{
				unitPositionsQueued.erase(unitPositionsQueued.begin() + i);
			}
		}
		if (prodsize > 0 && unitPositionsQueued.size() > 0 &&unitPositionsQueued[unitPositionsQueued.size() - 1] < prodsize)
		{
			linkedProdBuilding->productionQueue.erase(linkedProdBuilding->productionQueue.begin() + unitPositionsQueued[unitPositionsQueued.size() - 1]);
		}
		currstate = GUIDependencies::ElementState::IDLE;
		break;
	}
}
void ConstructorUnitElement::render(sf::RenderWindow* wind)
{
	wind->draw(visual);
}
ConstructorBuildingElement::ConstructorBuildingElement(double XC, double YC, double WIDTHC, double HEIGHTC, BuildingDependencies::BuildingType TYPEC, sf::Texture* tex, sf::RenderWindow* wind, BuildingDependencies* builddepC, MouseState* mousestateC)
{
	this->wind = wind;
	x = XC;
	y = YC;
	type = TYPEC;
	width = WIDTHC;
	height = HEIGHTC;
	visual.setTexture(*tex);
	builddep = builddepC;
	mousestate = mousestateC;
	//Multiplies by this screen ratio and the ideal screen ratio
	x *= (wind->getView().getSize().x / 1940);
	y *= (wind->getView().getSize().y / 1080);
	//sets the position of the visual, multiplying the percentages XC and YC by the size of the view
	visual.setPosition((wind->getView().getCenter().x - wind->getView().getSize().x / 2) + x, (wind->getView().getCenter().y - wind->getView().getSize().y / 2) + y);
	//Multiplied width by a ratio of the current screen size and the screen size this game was made for. By doing this, we set width and height GUI values to a value that multiple systems can use. We only have to code for 1940/1080, however
	width *= (wind->getView().getSize().x / 1940);
	//Same operation with width as with height
	height *= (wind->getView().getSize().y / 1080);
	//Sets visuals scale to width and height divided by the current bounds of visual. This will result in visual's bounds being identical to width and height when multiplied by its scale
	visual.setScale(width / visual.getLocalBounds().width, height / visual.getLocalBounds().height);
}
PhantomBuilding::PhantomBuilding(sf::Vector2f currMapLocationC, sf::Texture texC, int widthC, int heightC, MouseState* mousestate, ConstructorBuildingElement* builder)
{
	colliding = false;
	constBuilder = builder;
	mouse = mousestate;
	currMapLocation = currMapLocationC;
	width = widthC;
	height = heightC;
	visual.setTexture(texC);
	visual.setColor(sf::Color(255, 255, 255, 255));
	visual.setPosition(currMapLocation.x * 64, currMapLocation.y * 64);
	visual.setScale(widthC * 64 / visual.getLocalBounds().width, heightC * 64 / visual.getLocalBounds().height);
}
void PhantomBuilding::update(double delta, map* gamemap, sf::RenderWindow* wind)
{
	currMapLocation.x = wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).x / 64;
	currMapLocation.y = wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).y / 64;
	visual.setPosition(currMapLocation.x * 64, currMapLocation.y * 64);
	if (gamemap->retrieveUnits(currMapLocation.x, currMapLocation.y, width, height).size() > 0 || gamemap->retrieveBuildings(currMapLocation.x, currMapLocation.y, width, height).size() > 0)
	{
		colliding = true;
		visual.setColor(sf::Color::Red);
	}
	else
	{
		visual.setColor(sf::Color(255, 255, 255, 255));
		colliding = false;
	}
}
void PhantomBuilding::render(sf::RenderWindow* wind)
{
	wind->draw(visual);
}
void ConstructorBuildingElement::update(double delta)
{
	visual.setPosition((wind->getView().getCenter().x - wind->getView().getSize().x / 2) + x, (wind->getView().getCenter().y - wind->getView().getSize().y / 2) + y);
	switch (currstate)
	{
	case GUIDependencies::ElementState::LEFTCLICKED:
		switch (linkedEngineer->faction)
		{
		case UnitDependencies::UnitFaction::Modern:
			switch (type)
			{
			case BuildingDependencies::BuildingType::ModernFactory:
				currbuilding = new PhantomBuilding(sf::Vector2f(wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).x / 64,
				wind->mapPixelToCoords(sf::Mouse::getPosition((*wind))).y / 64), builddep->ModernFactoryTex, BuildingDependencies::ModernFactoryTileWidth, BuildingDependencies::ModernFactoryTileWidth, mousestate, this);
				mousestate->phantbuild = currbuilding;
				mousestate->currstate = MouseState::RenderingPhantomBuilding;
				break;
			}
			break;
		}		
		currstate = GUIDependencies::ElementState::IDLE;
		break;
	case GUIDependencies::ElementState::RIGHTCLICKED:
		/*
		for (int i = 0; i < buildingPositionsQueued.size(); i++)
		{
			if (i >= prodsize)
			{
				buildingPositionsQueued.erase(buildingPositionsQueued.begin() + i);
			}
		}
		for (int i = 0; i < phantbuilds.size(); i++)
		{
			if (buildingPositionsQueued[i] >= prodsize)
			{
				phantbuilds.erase(phantbuilds.begin() + i);
			}
		}
		linkedEngineer->productionQueue.erase(linkedEngineer->productionQueue.begin() + buildingPositionsQueued[buildingPositionsQueued.size() - 1]);
		linkedEngineer->buildLocQueue.erase(linkedEngineer->buildLocQueue.begin() + buildingPositionsQueued[buildingPositionsQueued.size()-1]);
		//We don't bounds check because we know that if a type is added to the queue, there is also a phantom building added to the queue. 
		*/
		currstate = GUIDependencies::ElementState::IDLE;
		break;
	}
	
}
void ConstructorBuildingElement::pushBackConstruction(double delta)
{
	linkedEngineer->currBuildingType = type;
	linkedEngineer->currconstruction = currbuilding;
	linkedEngineer->currstate = UnitDependencies::UnitStates::CONSTRUCTING;
}
void ConstructorBuildingElement::render(sf::RenderWindow* wind)
{
	wind->draw(visual);
}
TextElement::TextElement(double XC, double YC, double WIDTHC, double HEIGHTC, int fontsize, sf::Font* currfont, sf::RenderWindow* wind)
{
	this->wind = wind; 
	y = YC;
	x = XC;
	//Multiplies by this screen ratio and the ideal screen ratio
	x *= (wind->getView().getSize().x / 1940);
	y *= (wind->getView().getSize().y / 1080);
	//sets the position of the visual, multiplying the percentages XC and YC by the size of the view
	text.setPosition((wind->getView().getCenter().x - wind->getView().getSize().x / 2) + x, (wind->getView().getCenter().y - wind->getView().getSize().y / 2) + y);
	//Multiplied width by a ratio of the current screen size and the screen size this game was made for. By doing this, we set width and height GUI values to a value that multiple systems can use. We only have to code for 1940/1080, however
	width *= (wind->getView().getSize().x / 1940);
	//Same operation with width as with height
	height *= (wind->getView().getSize().y / 1080);
	//Sets visuals scale to width and height divided by the current bounds of visual. This will result in visual's bounds being identical to width and height when multiplied by its scale
	text.setScale(width / text.getLocalBounds().width, height / text.getLocalBounds().height);
	text.setFont(*currfont);
	text.setCharacterSize(fontsize);
}
void TextElement::update(double delta)
{
	switch (currstate)
	{
	case GUIDependencies::ElementState::LEFTCLICKED:
		currstate = GUIDependencies::ElementState::IDLE;
		break;
	case GUIDependencies::ElementState::RIGHTCLICKED:
		currstate = GUIDependencies::ElementState::IDLE;
		break;
	}
}
void TextElement::render(sf::RenderWindow* wind)
{
	wind->draw(text);
}
BackgroundElement::BackgroundElement(double XC, double YC, double WIDTHC, double HEIGHTC, sf::Texture* tex, sf::RenderWindow* wind)
{
	this->wind = wind;
	x = XC;
	y = YC;
	width = WIDTHC;
	height = HEIGHTC;
	visual.setTexture(*tex);
	//Multiplies by this screen ratio and the ideal screen ratio
	x *= (wind->getView().getSize().x / 1940);
	y *= (wind->getView().getSize().y / 1080);
	//sets the position of the visuals senter, subtracts half its size(width and height) to get top left, then adds x and y. 
	visual.setPosition((wind->getView().getCenter().x - wind->getView().getSize().x / 2)+x, (wind->getView().getCenter().y - wind->getView().getSize().y / 2)+y);
	//Multiplied width by a ratio of the current screen size and the screen size this game was made for. By doing this, we set width and height GUI values to a value that multiple systems can use. We only have to code for 1940/1080, however
	width *= (wind->getView().getSize().x / 1920);
	//Same operation with width as with height
	height *= (wind->getView().getSize().y / 1080);
	//Sets visuals scale to width and height divided by the current bounds of visual. This will result in visual's bounds being identical to width and height when multiplied by its scale
	visual.setScale(width / visual.getLocalBounds().width, height / visual.getLocalBounds().height);

}
void BackgroundElement::update(double delta)
{
	visual.setPosition((wind->getView().getCenter().x - wind->getView().getSize().x / 2) + x, (wind->getView().getCenter().y - wind->getView().getSize().y / 2) + y);
	switch (currstate)
	{
	case GUIDependencies::ElementState::LEFTCLICKED:
		currstate = GUIDependencies::ElementState::IDLE;
		break;
	case GUIDependencies::ElementState::RIGHTCLICKED:
		currstate = GUIDependencies::ElementState::IDLE;
		break;
	}
}
void BackgroundElement::render(sf::RenderWindow* wind)
{
	wind->draw(visual);
}
void Collection::update(double delta)
{
	for (int i = 0; i < values.size(); i++)
	{
		values[i]->update(delta);
	}
}
void Collection::render(sf::RenderWindow* wind)
{
	for (int i = 0; i < values.size(); i++)
	{
		values[i]->render(wind);
	}
}
void GUIManager::update(double delta)
{
	for (int i = 0; i < values.size(); i++)
	{
		values[i].update(delta);
	}
}
void GUIManager::render(sf::RenderWindow* wind)
{
	for (int i = 0; i < values.size(); i++)
	{
		values[i].render(wind);
	}
}