#pragma once
#include<SFML\Graphics.hpp>
#include"ObjStates.h"
#include"MapDefs.h"
#include"UnitDefs.h"
#include"CameraDefs.h"
#include<vector>

class gameLoop
{
public:
	UnitDependencies Depend;
	map gamemap;
	Camera cam;
	sf::RenderWindow wind;
	MouseState mouse;
	std::vector<sf::Event> EventStack;
	void Initialize();
	void handleEvents();
	void logic(double delta);
	void render();
	void run();
};