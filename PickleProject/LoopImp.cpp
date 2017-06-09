#include"stdafx.h"
#include"LoopDefs.h"
#include<iostream>
void gameLoop::Initialize()
{
	//windowsetup
	wind.create(sf::VideoMode(100, 100), "Project Pickle", sf::Style::Fullscreen);
	wind.setMouseCursorVisible(false);
	//End of window setup
	//map setup
	gamemap.create(1000, 1000);
	//map end
	//camera setup
	sf::Vector2f centerCam(0,0);
	centerCam.x = 0;
	centerCam.y = 0;
	cam.create(centerCam, &gamemap, wind.getSize().x, wind.getSize().y, &wind);
	wind.setView(cam.camview);
	//End of camera setup
	//Mouse setup
	mouse.create(&gamemap, &wind);
	//End of mouse setup
}
void gameLoop::handleEvents()
{
	sf::Event ev;
	EventStack.clear();
	while (wind.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
		{
			wind.close();
		}
		else if (ev.type == sf::Event::MouseButtonPressed)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
			{
				mouse.RightButtonDown = true; 
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				mouse.LeftButtonDown = true;
			}
		}
		else if (ev.type == sf::Event::MouseButtonReleased)
		{
			//making square will be set to false in the logic() function
			if (mouse.RightButtonDown)
			{
				mouse.RightButtonDown = false;
			}
			else if (mouse.LeftButtonDown)
			{
				mouse.LeftButtonDown = false;
			}
		}
		else if (ev.type == sf::Event::MouseMoved)
		{
			mouse.moved = true;
			mouse.Position.x = wind.mapPixelToCoords(sf::Mouse::getPosition(wind)).x;
			mouse.Position.y = wind.mapPixelToCoords(sf::Mouse::getPosition(wind)).y;
		}
		else if (ev.type == sf::Event::KeyPressed)
		{
			if (ev.key.code == sf::Keyboard::A)
			{
				cam.currstate = cam.ScrollingLeft;
			}
			else if(ev.key.code == sf::Keyboard::W)
			{
				cam.currstate = cam.ScrollingUp;
			}
			else if (ev.key.code == sf::Keyboard::S)
			{
				cam.currstate = cam.ScrollingDown;
			}
			else if (ev.key.code == sf::Keyboard::D)
			{
				cam.currstate = cam.ScrollingRight;
			}

		}
		else if (ev.type == sf::Event::KeyReleased)
		{

			if (ev.key.code == sf::Keyboard::A || ev.key.code == sf::Keyboard::S || ev.key.code == sf::Keyboard::W || ev.key.code == sf::Keyboard::D)
			{
				cam.currstate = cam.Idle;
			}
			//PUT ALL NON-CONTROLGROUPABLE KEYS BEFORE THIS POINT. THESE IF STATEMENTS INITIALIZE A CONTROL GROUP. PUTTING NON-CONTROLGROUPABLE KEYS HERE WILL RESULT IN A CONFLICT OF INTEREST
			else if (ev.key.code == sf::Keyboard::LControl)
			{
				mouse.currstate = MouseState::ReadyingControlGroup;
			}
			else if (mouse.currstate == MouseState::ReadyingControlGroup)
			{
				mouse.currstate = MouseState::CreatingControlGroup;
				mouse.currControlGroupKey = ev.key.code;
			}
			else
			{
				//This should almost always be last. Only move if you truly want controlgroupkeys to be processed before something
				for (int i = 0; i < mouse.controlgroups.size(); i++)
				{
					if (mouse.controlgroups[i].sig == ev.key.code)
					{
						mouse.currControlGroupKey = ev.key.code;
						mouse.currstate = MouseState::SelectingControlGroup;
					}
				}
			}
		}
	}
}
void gameLoop::logic(double delta)
{
	cam.update(delta);
	mouse.update(delta);
}
void gameLoop::render()
{
	wind.clear();
	cam.render();

	
	//draw shit here 
}
void gameLoop::run()
{
	T1BasicFighter TEST1D(UnitDependencies::UnitType::LeopardTank, UnitDependencies::UnitFaction::Modern, TileDependencies::Flatland, &gamemap, UnitDependencies::AttackType::Physical,
		100, 100, 1000, 1, 60, sf::Vector2f(1,1), &Depend.LeopardTex);
	T1BasicFighter TEST2D(UnitDependencies::UnitType::LeopardTank, UnitDependencies::UnitFaction::Modern, TileDependencies::Flatland, &gamemap, UnitDependencies::AttackType::Physical,
		100, 100, 1000, 1, 60, sf::Vector2f(1, 1), &Depend.LeopardTex);
	T1BasicFighter TEST3D(UnitDependencies::UnitType::LeopardTank, UnitDependencies::UnitFaction::Modern, TileDependencies::Flatland, &gamemap, UnitDependencies::AttackType::Physical,
		100, 100, 1000, 1, 60, sf::Vector2f(1, 1), &Depend.LeopardTex);
	T1BasicFighter TEST4D(UnitDependencies::UnitType::LeopardTank, UnitDependencies::UnitFaction::Modern, TileDependencies::Flatland, &gamemap, UnitDependencies::AttackType::Physical,
		100, 100, 1000, 1, 60, sf::Vector2f(1, 1), &Depend.LeopardTex);
	double frame = (1.0 / 60.0);
	sf::Clock gametime;
	while (wind.isOpen())
	{
		double delta = gametime.restart().asSeconds();
		handleEvents();
		if (delta > frame)
		{
			logic(delta);
			TEST1D.update(delta);
			TEST2D.update(delta);
			TEST3D.update(delta);
			TEST4D.update(delta);
		}
		else
		{
			logic(delta);
			TEST1D.update(delta);
			TEST2D.update(delta);
			TEST3D.update(delta);
			TEST4D.update(delta);
			render();
			TEST1D.render(&wind);
			TEST2D.render(&wind);
			TEST3D.render(&wind);
			TEST4D.render(&wind);
			mouse.render(&wind);
			wind.setView(cam.camview);
			wind.display();

		}
	}
}
/*
double t = 0.0;
double dt = 1 / 60.0;

double currentTime = hires_time_in_seconds();

while (!quit)
{
	double newTime = hires_time_in_seconds();
	double frameTime = newTime - currentTime;
	currentTime = newTime;

	while (frameTime > 0.0)
	{
		float deltaTime = min(frameTime, dt);
		integrate(state, t, deltaTime);
		frameTime -= deltaTime;
		t += deltaTime;
	}

	render(state);
}
double deltaT;
double newtime = gametime.getElapsedTime().asMilliseconds();
while (wind.isOpen())
{
double currenttime = gametime.getElapsedTime().asMilliseconds();
double framedur = newtime - currenttime;
while (framedur > 0.0)
{
if (framedur == frame || framedur < frame)
{
deltaT = framedur;
}
else
{

deltaT = frame;
}
*/