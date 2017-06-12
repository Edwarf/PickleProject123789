#include"stdafx.h"
#include<iostream>
#include"ObjectDependencyDefs.h"
UnitDependencies::UnitDependencies()

{
	if (!LeopardTex.loadFromFile(("LeopardTank.png")))
	{

		std::cout << "Error, could not load image.";
	}
}
GUIDependencies::GUIDependencies()
{
	if (!DefaultBackGroundGUITex.loadFromFile("DefaultBackGroundGUI.png"))
	{
		std::cout << "Error loading file";
	}
}
TileDependencies::TileDependencies()
{
	if (!GoldenRocksTex.loadFromFile("GoldenRocks.png"))
	{
		std::cout << "Error, could not load image.";
	}
}