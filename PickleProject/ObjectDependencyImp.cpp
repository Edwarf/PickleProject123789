#include"stdafx.h"
#include<iostream>
#include"ObjectDependencyDefs.h"
#include"GUIDefs.h"
#include"ObjStates.h"
BuildingDependencies::BuildingDependencies()
{
	if (!ModernFactoryTex.loadFromFile("ModernFactoryTex.png"))
	{
		std::cout << "Error, could not load image";
	}
}
UnitDependencies::UnitDependencies()
{
	if (!LeopardTex.loadFromFile(("LeopardTank.png")))
	{

		std::cout << "Error, could not load image.";
	}
	if (!ModernT1EngineeerTex.loadFromFile("ModernT1EngineerTex.png"))
	{
		std::cout << "Error, could not load image";
	}
	if (!ModernCommanderTex.loadFromFile("ModernCommanderTex.png"))
	{
		std::cout << "Error, could not load image.";
	}
}

//Modern Units
std::vector<Element*> GUIDependencies::GetModernLeopardTankUI(sf::RenderWindow* wind, BuildingDependencies* buildingdepend)
{
	std::vector<Element*> returner;
	//Dynamically allocates memory, make sure to delete with void DeleteGUIs(). 
	returner.push_back(new BackgroundElement(0, 768, 1920, 312, &ModernBackgroundBottomGUITex, wind));
	GUIs.push_back(returner);
	return returner;
}
std::vector<Element*> GUIDependencies::GetModernT1EngineerUI(sf::RenderWindow* wind, BuildingDependencies* buildingdepend, MouseState* mouse)
{
	std::vector<Element*> returner;
	//Dynamically allocates memory, make sure to delete
	returner.push_back(new BackgroundElement(0, 768, 1920, 312, &ModernBackgroundBottomGUITex, wind));
	returner.push_back(new ConstructorBuildingElement(0, 868, 200, 200, BuildingDependencies::BuildingType::ModernFactory, &buildingdepend->ModernFactoryTex, wind, buildingdepend, mouse));
	GUIs.push_back(returner);
	return returner;
}
//Modern Buildings
std::vector<Element*> GUIDependencies::GetModernFactoryUI(sf::RenderWindow* wind, UnitDependencies* unitdepend)
{
	std::vector <Element*> returner;
	//Dynamically allocates memory, make sure to delete with void DeleteGUIs(). 
	returner.push_back(new BackgroundElement(0, 768, 1920, 312, &ModernBackgroundBottomGUITex, wind));
	returner.push_back(new ConstructorUnitElement(0, 768, 200, 200, UnitDependencies::UnitType::ModernLeopardTank, &unitdepend->LeopardTex, wind));
	returner.push_back(new ConstructorUnitElement(501, 768, 200, 200, UnitDependencies::UnitType::ModernT1Engineer, &unitdepend->ModernT1EngineeerTex, wind));
	GUIs.push_back(returner);
	return returner;
}
GUIDependencies::GUIDependencies()
{
	if (!ModernBackgroundBottomGUITex.loadFromFile("ModernBackgroundBottomGUITex.png"))
	{
		std::cout << "Error loading file";
	}
}
void GUIDependencies::DeleteGUIs()
{
	//Iterates through every GUI element, deleting it. 
	for (int z = 0; z < GUIs.size(); z++)
	{
		for (int i = 0; i < GUIs[z].size(); i++)
		{
			delete GUIs[z][i];
		}
	}
}
TileDependencies::TileDependencies()
{
	if (!GoldenRocksTex.loadFromFile("GoldenRocks.png"))
	{
		std::cout << "Error, could not load image.";
	}
}
ProjectileDependencies::ProjectileDependencies()
{
	if (!StandardLaserRound.loadFromFile("StandardLaserRoundTex.png"))
	{
		std::cout << "Error, could not load image.";
	}
}