#pragma once
#include<vector>
#include<SFML\Graphics.hpp>
#include"ObjectDependencyDefs.h"
class MouseState;
//GUI classes don't use the tile system and are not bound to tiles.
class ProductionBuilding;
class Engineer;
class map;
class ConstructorBuildingElement;
class Element
{
public:
	//This is the building that we add units to queue in. It will be linked to its building by the building 
	ProductionBuilding* linkedProdBuilding = nullptr;
	//This is the building that we add engineers to queue in. It will be linked to its building by the building. 
	Engineer* linkedEngineer = nullptr;
	sf::RenderWindow* wind;
	double x;
	double y;
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

	//This variable defines the type of unit is returned when the element is created
	UnitDependencies::UnitType type;
	//XC, YC, and WIDTHC and HEIGHTC should be PERCENTAGES of the screen
public:
	ConstructorUnitElement(double XC, double YC, double WIDTHC, double HEIGHTC, UnitDependencies::UnitType TYPEC, sf::Texture* tex, sf::RenderWindow* wind);
	void update(double delta);
	void render(sf::RenderWindow* wind);
};
class PhantomBuilding
{
public:
	bool colliding;
	ConstructorBuildingElement* constBuilder;
	void update(double delta, map* gamemap, sf::RenderWindow* wind);
	void render(sf::RenderWindow* wind);
	MouseState* mouse;
	sf::Vector2f currMapLocation;
	sf::Sprite visual;
	int width;
	int height;
	PhantomBuilding(sf::Vector2f currMapLocation, sf::Texture tex, int width, int height, MouseState* mousestate, ConstructorBuildingElement* builder);
};
class ConstructorBuildingElement : public Element
{
	int sizeOfLastQueue;
	//This is a vector of the unit positions in the linked building. When we remove objects from the queue, we use the positions denoted here.
	std::vector<int> buildingPositionsQueued;
	//This variable defines the type of building is returned when the element is created
	BuildingDependencies::BuildingType type;
public:
	MouseState* mousestate;
	PhantomBuilding* currbuilding;
	BuildingDependencies* builddep;
	void pushBackConstruction(double delta);
	//XC, YC, and WIDTHC and HEIGHTC should be PERCENTAGES of the screen
	ConstructorBuildingElement(double XC, double YC, double WIDTHC, double HEIGHTC, BuildingDependencies::BuildingType TYPEC, sf::Texture* tex, sf::RenderWindow* wind, BuildingDependencies* builddepC, MouseState* mousestateC);
	void update(double delta);
	void render(sf::RenderWindow* wind);
public:
};
class TextElement : public Element
{
public:
	sf::Text text;
	TextElement(double XC, double YC, double WIDTHC, double HEIGHTC, int fontsize, sf::Font* currfont, sf::RenderWindow* wind);
	void update(double delta);
	void render(sf::RenderWindow* wind);
};
class BackgroundElement : public Element
{
public:
	BackgroundElement(double XC, double YC, double WIDTHC, double HEIGHTC, sf::Texture* tex, sf::RenderWindow* wind);
	void update(double delta);
	void render(sf::RenderWindow* wind);
};
class Collection
{
public:
	std::vector<Element*> values;
	void update(double delta);
	void render(sf::RenderWindow* wind);
};
class GUIManager
{
public:
	std::vector<Collection> values;
	Collection* currcollec;
	void update(double delta);
	void render(sf::RenderWindow* wind);
};
