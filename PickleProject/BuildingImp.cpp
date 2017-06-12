#include"stdafx.h"
#include"BuildingDefs.h"
#include<vector>
void Building::provideVision(double delta)
{
	std::vector<tile*> visiontiles = gamemap->returnMapSquare(location.x - tilewidth/2, location.y - tileheight/2, visionSquare+(tilewidth/2 + tilewidth), visionSquare+(tileheight/2+tileheight));
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
	}
	//Adjusting production rates. 
	if (gamemap->teams[team].totalKaskan < 0)
	{
		int absolutevaluetotkask = gamemap->teams[team].totalKaskan*-1;
		//this takess the difference of the kaskan capacity and kaskan deficit. It then divides it by the capacity, obtaining a rate at which we multiply the production rate.
		productionRate *= gamemap->teams[team].KaskanCapacity - absolutevaluetotkask / gamemap->teams[team].KaskanCapacity;
	}
	else
	{
		productionRate = origProductionRate;
	}
	if (gamemap->teams[team].totalIridium < 0)
	{
		int absolutevaluetotirid = gamemap->teams[team].totalIridium*-1;
		//this takess the difference of the iridium capacity and iridium deficit. It then divides it by the capacity, obtaining a rate at which we multiply the production rate.
		productionRate *= gamemap->teams[team].IridiumCapacity - absolutevaluetotirid / gamemap->teams[team].IridiumCapacity;
	}
	else
	{
		productionRate = origProductionRate;
	}
	double effectiveProductionRate = productionRate*delta;
	//Producing units
	switch (currProducingUnitType)
	{
	case UnitDependencies::UnitType::LeopardTank:
		if (currentlyProducedIridium < UnitDependencies::LeopardTankIridiumCost)
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
		if (currentlyProducedKaskan < UnitDependencies::LeopardTankKaskanCost)
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
			gamemap->teams[team].TeamUnits.push_back(new T1BasicFighter(UnitDependencies::UnitType::LeopardTank, UnitDependencies::UnitFaction::Modern, TileDependencies::tileType::Flatland, gamemap, UnitDependencies::AttackType::Physical, UnitDependencies::LeopardTankHP, 
			UnitDependencies::LeopardTankIridiumCost, UnitDependencies::LeopardTankKaskanCost, UnitDependencies::LeopardTankArmor,UnitDependencies::LeopardTankSpeed, sf::Vector2f(location.x + tilewidth/2, location.y + tileheight + 1)
			,&UnitDepend->LeopardTex, guidep, wind, UnitDependencies::LeopardTankVisionRange, UnitDependencies::LeopardTankRange,UnitDependencies::LeopardTankRof,UnitDependencies::LeopardTankDamage));
			//Reset values, with exception of production rate
			currentlyProducedIridium = 0;
			currentlyProducedKaskan = 0;
			sufficientIridium = false;
			sufficientKaskan = false;
		}
		break;
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
	sf::RenderWindow* windC)
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
	UnitDepend = unitdependC;
	//End of parameter setting
	//Custom Initialization
	visual.setTexture(*tex);
	visual.setPosition(location.x * 64, location.y * 64);
	visual.setScale(tilewidth * 64 / visual.getLocalBounds().width, tileheight * 64 / visual.getLocalBounds().height);
	std::vector<tile*> tempTileMap = gamemap->returnMapSquare(location.x, location.y, tilewidth, tileheight);
	//initializes on map
	for (int i = 0; i < tempTileMap.size(); i++)
	{
		tempTileMap[i]->currbuilding = this;
	}

}
