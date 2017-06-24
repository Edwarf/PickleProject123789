#include"stdafx.h"
#include"MapDefs.h"
#include"UnitDefs.h"
#include<iostream>
#include"ObjectDependencyDefs.h"
tile::tile(sf::Texture* tex, sf::Vector2f newpos, TileDependencies::tileType tp)
{
	pos = newpos;
	visual.setTexture((*tex));
	visual.setPosition(newpos.x, newpos.y);
	type = tp;
	origColor = visual.getColor();
	visual.setColor(sf::Color(255, 255, 255, 100));
	resetColor = sf::Color(255, 255, 255, 100);
}
void tile::update(double delta)
{
	switch (currstate)
	{
	case TileDependencies::TileState::IDLE:
		visual.setColor(sf::Color(255, 255, 255, 100));
		resetColor = sf::Color(255, 255, 255, 100);
		break;
	case TileDependencies::TileState::VISIBLE:
		visual.setColor(origColor);
		currstate = TileDependencies::TileState::REVEALED;
		//sets reset color to IDLE's color
		resetColor = sf::Color(255, 255, 255, 100);
		break;
	}
	currTeamsSeeing.clear();
}
void tile::render(sf::RenderWindow* wind)
{
	wind->draw(visual);
	visual.setColor(resetColor);
}
map::map(int height, int width, int numberOfTeams, UnitDependencies* unitdepC, TileDependencies* tiledepC, BuildingDependencies* buildingdepC, GUIDependencies* guidepC, ProjectileDependencies* projectdependC, sf::RenderWindow* win, MouseState* mouseC)
{
	unitdep = unitdepC;
	tiledep = tiledepC;
	buildingdep = buildingdepC;
	guidep = guidepC;
	mouse = mouseC;
	projectdepend = projectdependC;
	sf::Vector2f vec(0, 0);
	std::vector<tile> temp;
	for (int i = 0; i < width; i++)
	{
		for (int z = 0; z < height; z++)
		{
			temp.push_back(tile(&Dependencies.GoldenRocksTex, vec, TileDependencies::tileType::Flatland));
			vec.y += 64;
		}
		vec.y = 0;
		tilegrid.push_back(temp);
		temp.clear();
		vec.x += 64;
	}
	for (int i = 0; i < numberOfTeams; i++)
	{
		teams.push_back(Team(unitdep, tiledep, buildingdep, guidep, projectdepend, this, i, win, mouse));
	}
}
void map::create(int height, int width, int numberOfTeams, UnitDependencies* unitdepC, TileDependencies* tiledepC, BuildingDependencies* buildingdepC, GUIDependencies* guidepC, ProjectileDependencies* projectdependC ,sf::RenderWindow* win, MouseState* mouseC)
{
	unitdep = unitdepC;
	tiledep = tiledepC;
	buildingdep = buildingdepC;
	guidep = guidepC;
	projectdepend = projectdependC;
	mouse = mouseC;
	sf::Vector2f vec(0, 0);
	std::vector<tile> temp;
	for (int i = 0; i < width; i++)
	{
		for (int z = 0; z < height; z++)
		{
			temp.push_back(tile(&Dependencies.GoldenRocksTex, vec, TileDependencies::tileType::Flatland));
			vec.y += 64;
		}
		vec.y = 0;
		tilegrid.push_back(temp);
		temp.clear();
		vec.x += 64;
	}
	for (int i = 0; i < numberOfTeams; i++)
	{
		teams.push_back(Team(unitdep, tiledep, buildingdep, guidep, projectdepend, this, i, win, mouse));
	}
}
map::map()
{

}
void map::update(double delta)
{
	for (int i = 0; i < teams.size(); i++)
	{
		teams[i].update(delta);
	}
	for (auto i : tileUpdates)
	{
		i->update(delta);
	}
	tileUpdates.clear();

}
void map::render(sf::RenderWindow* window)
{
	for (int i = 0; i < tilegrid.size(); i++)
	{
		for (int z = 0; z < tilegrid[i].size(); z++)
		{
			tilegrid[i][z].render(window);
		}
	}
	for (int i = 0; i < teams.size(); i++)
	{
		teams[i].render(window);
	}
}
void map::renderpart(sf::RenderWindow* window, int x, int y, int width, int height)
{
	for (int i = x; i < width + x; i++)
	{
		for (int z = y; z < height + y; z++)
		{
			tilegrid[i][z].render(window);
		}
	}
	for (int i = 0; i < teams.size(); i++)
	{
		teams[i].render(window);
	}
}
std::vector<Unit*> map::retrieveUnits(int x, int y, int height, int width)
{
	std::vector<Unit*> returner;
	//Widthx utilizes bounds checking and is safe. Don't use raw values unless aware of the lack of bounds checking.
	int widthx = width + x;
	//Heighty utilizes bounds checking and is safe. Don't use raw values unless aware of the lack of bounds checking.
	int heighty = height + y;
	if (widthx > tilegrid.size())
	{
		widthx = tilegrid.size() - 1;
	}
	if (widthx < 0)
	{
		widthx = 1;
	}
	//Tile grid is a rectangle, using 0 as access value for column because all are identical and it doesn't matter
	if (heighty > tilegrid[0].size())
	{
		heighty = tilegrid[0].size() - 1;

	}
	if (heighty < 0)
	{
		heighty = 1;
	}

	if (std::sqrt(std::pow(height, 2)) / height == 1)
	{
		for (int i = x; i < widthx; i++)
		{
			for (int z = y; z < heighty; z++)
			{
				if (tilegrid[i][z].currunit != nullptr)
				{
					returner.push_back(tilegrid[i][z].currunit);
				}
			}
		}
	}
	return returner;
}
std::vector<Building*> map::retrieveBuildings(int x, int y, int height, int width)
{	
	std::set<Building*> initialreturner;
	//Widthx utilizes bounds checking and is safe. Don't use raw values unless aware of the lack of bounds checking.
	int widthx = width + x;
	//Heighty utilizes bounds checking and is safe. Don't use raw values unless aware of the lack of bounds checking.
	int heighty = height + y;
	if (widthx > tilegrid.size())
	{
		widthx = tilegrid.size() - 1;
	}
	if (widthx < 0)
	{
		widthx = 1;
	}
	//Tile grid is a rectangle, using 0 as access value for column because all are identical and it doesn't matter
	if (heighty > tilegrid[0].size())
	{
		heighty = tilegrid[0].size() - 1;

	}
	if (heighty < 0)
	{
		heighty = 1;
	}
	if (std::sqrt(std::pow(height, 2)) / height == 1)
	{
		for (int i = x; i < widthx; i++)
		{
			for (int z = y; z < heighty; z++)
			{
				if (tilegrid[i][z].currbuilding != nullptr)
				{
					initialreturner.insert(tilegrid[i][z].currbuilding);
				}
			}
		}
	}
	//conver the set to a vector
	std::vector<Building*> returner;
	for (auto i : initialreturner)
	{
		returner.push_back(i);
	}
	return returner;
}
std::vector<tile*> map::returnMapSquare(int x, int y, int height, int width)
{

	std::vector<tile*> returner;
	int widthx = width + x;
	int heighty = height + y;
	if (x < 0)
	{
		x = 1;
	}
	if (y < 0)
	{
		y = 1;
	}
	if (x > tilegrid.size())
	{
		x = tilegrid.size() - 1;
	}
	if (y > tilegrid[0].size())
	{
		y = tilegrid[0].size() - 1;
	}
	if (widthx > tilegrid.size())
	{
		widthx = tilegrid.size() - 1;
	}
	if (heighty > tilegrid[0].size())
	{
		heighty = tilegrid[0].size() - 1;
	}
	for (int i = x; i <= widthx; i++)
	{
		for (int z = y; z <= heighty; z++)
		{
			returner.push_back(&tilegrid[i][z]);
			tileUpdates.insert(&tilegrid[i][z]);
		}
	}
	return returner;
}
//RETURNS NULL IF OUT OF BOUNDS. DON'T USE WITHOUT CHECKING FOR A NULL RESULT
tile* map::returnTile(int x, int y)
{
	if (x < 0 || y < 0)
	{
		return nullptr;
	}
	else if (x > tilegrid.size() || y > tilegrid[0].size())
	{
		return nullptr;
	}
	else
	{
		return &tilegrid[x][y];
	}
}