#include"stdafx.h"
#include"MapDefs.h"
#include<iostream>
TileDependencies::TileDependencies()
{
	if (!grasstex.loadFromFile(("GoldenRocks.png"), sf::IntRect(0,0,64,64)));
	{		
		std::cout << "Error, could not load image.";
	}
}
tile::tile(sf::Texture* tex, sf::Vector2f newpos, TileDependencies::tileType tp)
{
	pos = newpos;
	visual.setTexture((*tex));
	visual.setPosition(newpos.x, newpos.y);
	type = tp;
}
void tile::update(sf::Time delta)
{

}
void tile::render(sf::RenderWindow* wind)
{
	wind->draw(visual);
}
map::map(int height, int width)
{
	sf::Vector2f vec(0, 0);
	std::vector<tile> temp;
	for (int i = 0; i < width; i++)
	{
		for (int z = 0; z < height; z++)
		{
			temp.push_back(tile(&Dependencies.grasstex, vec, TileDependencies::tileType::Flatland));
			vec.y += 64;
		}
		vec.y = 0;
		tilegrid.push_back(temp);
		temp.clear();
		vec.x += 64;
	}
}
void map::create(int height, int width)
{
	sf::Vector2f vec(0, 0);
	std::vector<tile> temp;
	for (int i = 0; i < width; i++)
	{
		for (int z = 0; z < height; z++)
		{
			temp.push_back(tile(&Dependencies.grasstex, vec, TileDependencies::tileType::Flatland));
			vec.y += 64;
		}
		vec.y = 0;
		tilegrid.push_back(temp);
		temp.clear();
		vec.x += 64;
	}
}
map::map()
{

}
void map::update(double delta)
{
	for (int i = 0; i < tilegrid.size(); i++)
	{
		for (int z = 0; z < tilegrid[i].size(); z++)
		{

		}
	}
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