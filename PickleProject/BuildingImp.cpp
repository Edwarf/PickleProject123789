#include"stdafx.h"
#include"BuildingDefs.h"
#include"GUIDefs.h"
#include<vector>
//Put this function after definitions of faction and unittype. Ideally, it will be the last function called in the constructor 
void Building::DefaultUISetup(GUIDependencies* gui, sf::RenderWindow* wind)
{
	switch (faction)
	{
	case UnitDependencies::UnitFaction::Modern:
		switch (type)
		{
		case BuildingDependencies::BuildingType::ModernFactory:
			UIBuilding.values = gui->GetModernFactoryUI(wind, unitdep);
			break;
		}
		break;
	}
	CustomPrepUI();
}

void Building::provideVision(double delta)
{
	std::vector<tile*> visiontiles = gamemap->returnMapSquare(location.x - tilewidth, location.y - tileheight, visionSquare+(tilewidth/2 + tilewidth), visionSquare+(tileheight/2+tileheight));
	for (int i = 0; i < visiontiles.size(); i++)
	{
		visiontiles[i]->currstate = TileDependencies::TileState::VISIBLE;
		visiontiles[i]->currTeamsSeeing.insert(team);
	}
}
void UnitProductionBuilding::render(sf::RenderWindow * win)
{
	win->draw(visual);
}

void UnitProductionBuilding::update(double delta)
{
	if (HP <= 0)
	{
		currstate = BuildingDependencies::DEAD;
	}
	if (productionQueue.size() > 0)
	{
		currstate = BuildingDependencies::ACTING;
	}
	else
	{
		currstate = BuildingDependencies::IDLE;
	}
	switch (currstate)
	{
	case BuildingDependencies::BuildingStates::ACTING:
		produce(delta);
		currstate = BuildingDependencies::BuildingStates::IDLE;
		break;
	}
	provideVision(delta);
}
void UnitProductionBuilding::produce(double delta)
{
	//If production is not equal to productionqueue[0], user has canceled production. In that case, reset all production stats and set the current production type to productionqueue[0]
	if (currProducingUnitType != productionQueue[0])
	{
		currProducingUnitType = productionQueue[0];
		currentlyProducedIridium = 0;
		currentlyProducedKaskan = 0;
		sufficientIridium = false;
		sufficientKaskan = false;
	}
	//Adjusting production rates. 
	if (gamemap->teams[team].totalKaskan < 0)
	{
		int absolutevaluetotkask = gamemap->teams[team].totalKaskan*-1;
		//this takess the difference of the kaskan capacity and kaskan deficit. It then divides it by the capacity, obtaining a rate at which we multiply the production rate.
		productionRate *= gamemap->teams[team].KaskanCapacity - absolutevaluetotkask / gamemap->teams[team].KaskanCapacity;
	}
	if (gamemap->teams[team].totalIridium < 0)
	{
		int absolutevaluetotirid = gamemap->teams[team].totalIridium*-1;
		//this takess the difference of the iridium capacity and iridium deficit. It then divides it by the capacity, obtaining a rate at which we multiply the production rate.
		productionRate *= gamemap->teams[team].IridiumCapacity - absolutevaluetotirid / gamemap->teams[team].IridiumCapacity;
	}
	double effectiveProductionRate = productionRate*delta;
	//Producing units
	switch (currProducingUnitType)
	{
	case UnitDependencies::UnitType::ModernLeopardTank:
		if (currentlyProducedIridium < UnitDependencies::ModernLeopardTankIridiumCost)
		{
			//adds iridium to the currently produced unit
			currentlyProducedIridium += effectiveProductionRate;
			//takes iridium away from team economy
			gamemap->teams[team].totalIridium -= effectiveProductionRate;
		}
		else
		{
			sufficientIridium = true;
		}
		if (currentlyProducedKaskan < UnitDependencies::ModernLeopardTankKaskanCost)
		{
			currentlyProducedKaskan += effectiveProductionRate;
			gamemap->teams[team].totalKaskan -= effectiveProductionRate;
		}
		else
		{
			sufficientKaskan = true;
		}
		if (sufficientIridium && sufficientKaskan)
		{
			//Produce the leopard tank, place it outside of building. 
			gamemap->teams[team].TeamUnits.push_back(new T1BasicFighter(UnitDependencies::UnitType::ModernLeopardTank, UnitDependencies::UnitFaction::Modern, TileDependencies::tileType::Flatland, gamemap, UnitDependencies::ModernLeopardTankAttackType, UnitDependencies::ModernLeopardTankHP, 
			UnitDependencies::ModernLeopardTankIridiumCost, UnitDependencies::ModernLeopardTankKaskanCost, UnitDependencies::ModernLeopardTankArmor,UnitDependencies::ModernLeopardTankSpeed, NextOpenPosition()
			,&unitdep->LeopardTex, guidep, wind, UnitDependencies::ModernLeopardTankVisionRange, UnitDependencies::ModernLeopardTankRange,UnitDependencies::ModernLeopardTankRof,UnitDependencies::ModernLeopardTankDamage, builddep, projectDepend, team, mouse));
			//Reset values, with exception of production rate
			currentlyProducedIridium = 0;
			currentlyProducedKaskan = 0;
			sufficientIridium = false;
			sufficientKaskan = false;
			productionQueue.erase(productionQueue.begin());
		}
		break;
	case UnitDependencies::UnitType::ModernT1Engineer:
		if (currentlyProducedIridium < UnitDependencies::ModernT1EngineerIridiumCost)
		{
			//adds iridium to the currently produced unit
			currentlyProducedIridium += effectiveProductionRate;
			//takes iridium away from team economy
			gamemap->teams[team].totalIridium -= effectiveProductionRate;
		}
		else
		{
			sufficientIridium = true;
		}
		if (currentlyProducedKaskan < UnitDependencies::ModernT1EngineerKaskanCost)
		{
			currentlyProducedKaskan += effectiveProductionRate;
			gamemap->teams[team].totalKaskan -= effectiveProductionRate;
		}
		else
		{
			sufficientKaskan = true;
		}
		if (sufficientIridium && sufficientKaskan)
		{

			//Produce the leopard tank, place it outside of building. 
			gamemap->teams[team].TeamUnits.push_back(new T1Engineer(UnitDependencies::UnitType::ModernT1Engineer, UnitDependencies::UnitFaction::Modern, TileDependencies::Flatland, TileDependencies::tileType::Aquatic, gamemap, UnitDependencies::ModernT1EngineerAttackType,
				UnitDependencies::ModernT1EngineerHP, UnitDependencies::ModernT1EngineerIridiumCost, UnitDependencies::ModernT1EngineerKaskanCost, UnitDependencies::ModernT1EngineerArmor, UnitDependencies::ModernLeopardTankSpeed, NextOpenPosition(),
				&unitdep->ModernT1EngineeerTex, guidep, wind, UnitDependencies::ModernT1EngineerVisionRange, UnitDependencies::ModernT1EngineerRange, UnitDependencies::ModernT1EngineerProductionRate, team, builddep, unitdep, projectDepend, mouse));
			//Reset values, with exception of production rate
			currentlyProducedIridium = 0;
			currentlyProducedKaskan = 0;
			sufficientIridium = false;
			sufficientKaskan = false;
			productionQueue.erase(productionQueue.begin());
		}
		break;
	}
	productionRate = origProductionRate;
}
//Potentially a massive cpu eater, make sure to optimize later
sf::Vector2f UnitProductionBuilding::NextOpenPosition()
{
	sf::Vector2f returner;
	bool found = false;
	tile* currtile;
	int i = 0;
	while (found == false)
	{
		for (i; i < UnitTileStartingPositions.size(); i++)
		{
			currtile = &gamemap->tilegrid[UnitTileStartingPositions[i].x][UnitTileStartingPositions[i].y];
			if (currtile->currbuilding == nullptr && currtile->currunit == nullptr)
			{
				found = true;
				returner = UnitTileStartingPositions[i];
			}
		}
		if (found == false)
		{
			startingPositionY++;
			for (int z = location.x; z <= location.x + tilewidth; z++)
			{
				UnitTileStartingPositions.push_back(sf::Vector2f(z, location.y + tileheight + startingPositionY));
			}
		}
	}
	return returner;
}
void ProductionBuilding::CustomPrepUI()
{
	for (int i = 0; i < UIBuilding.values.size(); i++)
	{
		UIBuilding.values[i]->linkedProdBuilding = this;
	}
}
//All bounds checking(position, width, height etc.) must be done outside of the building functions. 
UnitProductionBuilding::UnitProductionBuilding(int teamC, 
	UnitDependencies::UnitFaction factionC, 
	double HPC, 
	double ArmorC, 
	int IridiumCostC,
	int KaskanCostC, 
	double productionRateC, 
	map* gamemapC, 
	int tierC,
	int tileHeightC, 
	int tileWidthC, 
	int visionRadiusC, 
	sf::Vector2i locC,
	sf::Texture* tex, 
	GUIDependencies* guidepC, 
	UnitDependencies* unitdependC,
	BuildingDependencies* buildingdependC,
	sf::RenderWindow* windC,
	std::vector<Element*> elems,
	BuildingDependencies::BuildingType typeC,
	MouseState* mouseC,
	ProjectileDependencies* projectDependC)
{
	//Parameter setting
	team = teamC;
	faction = factionC;
	HP = HPC;
	Armor = ArmorC;
	IridiumCost = IridiumCostC;
	KaskanCost = KaskanCostC;
	productionRate = productionRateC;
	origProductionRate = productionRate;
	gamemap = gamemapC;
	tier = tierC;
	tileheight = tileHeightC;
	tilewidth = tileWidthC;
	visionSquare = visionRadiusC;
	location = locC;
	guidep = guidepC;
	wind = windC;
	unitdep = unitdependC;
	builddep = buildingdependC;
	type = typeC;
	mouse = mouseC;
	UIBuilding.values = elems;
	startingPositionY = 1;
	projectDepend = projectDependC;
	//End of parameter setting
	//Custom Initialization
	for (int i = location.x; i <= location.x + tilewidth; i++)
	{
		UnitTileStartingPositions.push_back(sf::Vector2f(i, location.y + tileheight + startingPositionY));
	}
	visual.setTexture(*tex);
	visual.setScale(tilewidth * 64 / visual.getLocalBounds().width, tileheight * 64 / visual.getLocalBounds().height);
	visual.setPosition(location.x * 64, location.y * 64);
	std::vector<tile*> tempTileMap = gamemap->returnMapSquare(location.x, location.y, tilewidth, tileheight);
	//initializes on map
	for (int i = 0; i < tempTileMap.size(); i++)
	{
		tempTileMap[i]->currbuilding = this;
	}
	DefaultUISetup(guidep, wind);
}
