#include"stdafx.h"
#include<SFML\Graphics.hpp>
#include"ObjectDependencyDefs.h"
#include<iostream>
UnitDependencies::UnitDependencies()

{

	if (!LeopardTex.loadFromFile(("LeopardTank.png")));

	{

		std::cout << "Error, could not load image.";

	}

}
GUIDependencies::GUIDependencies()
{
	if (!DefaultBackGroundGUITex.loadFromFile("DefaultBackGroundGUI.jpg"))
	{
		std::cout << "Error loading file";
	}
}
TileDependencies::TileDependencies()
{
	if (!grasstex.loadFromFile("Grass.jpg"));
	{
		std::cout << "Error, could not load image.";
	}
	if (!GoldenRocksTex.loadFromFile("GoldenRocks.png"))
	{
		std::cout << "Error, could not load image.";
	}
}