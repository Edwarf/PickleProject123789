#pragma once
#include"SFML\Graphics.hpp"
#include"ObjectDependencyDefs.h"
class map;
class Unit;
class Projectile
{
	Unit* linkedUnit;
public:
	ProjectileDependencies::ProjectileState currstate;
	sf::FloatRect ActiveRect;
	TileDependencies::tileType ttype;
	sf::Sprite visual;
	UnitDependencies::AttackType  type;
	int speed;
	map* gamemap;
	sf::Vector2f location;
	sf::Vector2f desiredPos;
	sf::Vector2f direction;
	sf::Vector2f MoveInterval;
	double dmg;
	int team;
	virtual void update(double delta);
	virtual void move(double delta);
	void render(sf::RenderWindow* wind);
	Projectile(UnitDependencies::AttackType typec, int speedC, map* gamemapC, double dmgC, int teamC, sf::Vector2f locC, sf::Vector2f despiredposC, sf::Texture* texC, Unit* linkedUnitC, TileDependencies::tileType ttypeC, ProjectileDependencies::ProjectileState currstateC);
};
class StandardTankRound : public Projectile
{

};