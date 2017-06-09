#pragma once
#include<vector>
#include"UnitDefs.h"
#include"BuildingDefs.h"
//GUI classes don't use the tile system and are not bound to tiles.
class GUIDependencies
{
public:
	enum ElementState
	{
		IDLE,
		LEFTCLICKED,
		RIGHTCLICKED,
	};
};
class Element
{
public:
	GUIDependencies::ElementState currstate = GUIDependencies::ElementState::IDLE;
	sf::Sprite visual;
	//Express width as a percentage of the screen in the constructor.
	double width;
	//Express height as a percentage of the screen in the constructor.
	double height;
	virtual void update(double delta) = 0;
	virtual void render(sf::RenderWindow* wind) = 0;
};
//Element of GUI. When clicked, pushes back a message to produce new units to the holding object.
class ConstructorUnitElement : public Element
{
	int sizeOfLastQueue;
	//This is a vector of the unit positions in the linked building. When we remove objects from the queue, we use the positions denoted here.
	std::vector<int> unitPositionsQueued;
	//This is the building that we add units to queue in 
	ProductionBuilding* linkedProdBuilding;
	//This variable defines the type of unit returned when the element is created
	UnitDependencies::UnitType type;
	//XC, YC, and WIDTHC and HEIGHTC should be PERCENTAGES of the screen
public:
	ConstructorUnitElement(double XC, double YC, double WIDTHC, double HEIGHTC, UnitDependencies::UnitType TYPEC, sf::Texture* tex, sf::RenderWindow* wind);
	void update(double delta);
	void render(sf::RenderWindow* wind);
};
class Collection
{
	std::vector<Element*> values;
	void update(double delta);
	void render(sf::RenderWindow* wind);
};
class GUIManager
{
	std::vector<Collection> values;
};
