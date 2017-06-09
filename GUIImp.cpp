#include"stdafx.h"
#include"GUIDefs.h"
ConstructorUnitElement::ConstructorUnitElement(double XC, double YC, double WIDTHC, double HEIGHTC, UnitDependencies::UnitType TYPEC, sf::Texture* tex, sf::RenderWindow* wind)
{
	type = TYPEC;
	width = WIDTHC;
	height = HEIGHTC;
	visual.setTexture(*tex);
	//sets the position of the visual, multiplying the percentages XC and YC by the size of the view
	visual.setPosition(XC*wind->getView().getSize().x, YC*wind->getView().getSize().y);
	//sets the scale to 
	visual.setScale(width*wind->getView().getSize().x / visual.getLocalBounds().width, height*wind->getView().getSize().y / visual.getLocalBounds().height);
}
void ConstructorUnitElement::update(double delta)
{
	int prodsize = linkedProdBuilding->productionQueue.size();

	switch (currstate)
	{
	case GUIDependencies::ElementState::LEFTCLICKED:
		linkedProdBuilding->productionQueue.push_back(type);
		unitPositionsQueued.push_back(prodsize - 1);
		currstate = GUIDependencies::IDLE;
		break;
	case GUIDependencies::ElementState::RIGHTCLICKED:
		for (int i = 0; i < unitPositionsQueued.size(); i++)
		{
			if (unitPositionsQueued[i] >= prodsize)
			{
				unitPositionsQueued.erase(unitPositionsQueued.begin() + i);
			}
		}
		linkedProdBuilding->productionQueue.erase(linkedProdBuilding->productionQueue.begin() + unitPositionsQueued[unitPositionsQueued.size() - 1]);
		break;
	}
}
void ConstructorUnitElement::render(sf::RenderWindow* wind)
{
	wind->draw(visual);
}
void Collection::update(double delta)
{
	for (int i = 0; i < values.size(); i++)
	{
		values[i]->update(delta);
	}
}
void Collection::render(sf::RenderWindow* wind)
{
	for (int i = 0; i < values.size(); i++)
	{
		values[i]->render(wind);
	}
}