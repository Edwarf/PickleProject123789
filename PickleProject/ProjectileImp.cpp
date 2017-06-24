#include"stdafx.h"
#include"ProjectileDefs.h"
#include"UnitDefs.h"
#include"BuildingDefs.h"
Projectile::Projectile(UnitDependencies::AttackType typeC, int speedC, map* gamemapC, double dmgC, int teamC, sf::Vector2f locC, sf::Vector2f despiredPosC, sf::Texture* texC, Unit* linkedUnitC, TileDependencies::tileType ttypeC, ProjectileDependencies::ProjectileState currstateC)
{
	team = teamC;
	currstate = currstateC;
	ttype = ttypeC;
	linkedUnit = linkedUnitC;
	desiredPos = despiredPosC;
	type = typeC;
	speed = speedC;
	gamemap = gamemapC;
	dmg = dmgC;
	location = locC;
	visual.setTexture(*texC);
	visual.setPosition(location.x * 64, location.y * 64);
	visual.setOrigin(32, 32);
	visual.setScale(64 / visual.getLocalBounds().width, 64 / visual.getLocalBounds().height);
	//makes a rectangle with top left corner at current position and with bounds of the absolute value 
	ActiveRect = sf::FloatRect(location.x - 10, location.y - 10, 20, 20);
	direction = normalize(desiredPos - location);
	
}
void Projectile::update(double delta)
{
	if (linkedUnit != nullptr)
	{
		if (linkedUnit->currstate == UnitDependencies::DEAD)
		{
			linkedUnit = nullptr;
		}
	}
	switch (currstate)
	{
	case ProjectileDependencies::ProjectileState::MOVING:
		move(delta);
		break;
	}
}
void Projectile::move(double delta)
{
	MoveInterval = sf::Vector2f(direction.x*speed*delta, direction.y*speed*delta);
	location += MoveInterval;
	//	sf::Vector3f linedist(std::sqrt(std::pow(desiredpos.x - positionfloat.x, 2)), std::sqrt(std::pow(desiredpos.y - positionfloat.y, 2)), std::sqrt(std::pow(desiredpos.x - positionfloat.x, 2) + std::pow(desiredpos.y - positionfloat.y, 2)));
	double currentAngle = visual.getRotation();
	double targetAngle = ((180 / 3.14)*(std::atan2(desiredPos.y - location.y, desiredPos.x - location.x))) - 90;
	double realAngle = targetAngle - currentAngle;
	visual.rotate(realAngle);
	visual.setPosition(location.x*64, location.y*64);
	std::vector<tile*> currtile = gamemap->returnMapSquare(location.x - 2, location.y - 2, 4, 4);
	for (int i = 0; i < currtile.size(); i++)
	{
		if (currtile[i] != nullptr)
		{
			if (currtile[i]->currunit != nullptr && currtile[i]->currunit->visual.getGlobalBounds().contains(location.x*64, location.y*64) && currtile[i]->currunit->team != team)
			{
				int effectivedmg = (dmg - currtile[i]->currunit->Armor);
				if (effectivedmg > 0)
				{
					currtile[i]->currunit->HP -= effectivedmg;
				}
				currstate = ProjectileDependencies::ProjectileState::DEAD;
				break;
			}
			else if (currtile[i]->currbuilding != nullptr && currtile[i]->currbuilding->visual.getGlobalBounds().contains(location.x*64, location.y*64) && currtile[i]->currbuilding->team != team)
			{
				int effectivedmg = (dmg - currtile[i]->currbuilding->Armor);
				if (effectivedmg > 0)
				{
					currtile[i]->currbuilding->HP -= effectivedmg;
				}
				currstate = ProjectileDependencies::ProjectileState::DEAD;
				break;
			}
		}
	}
	if (!ActiveRect.contains(location))
	{
		currstate = ProjectileDependencies::ProjectileState::DEAD;
	}
}
void Projectile::render(sf::RenderWindow* wind)
{
	wind->draw(visual);
}