#pragma once
#include "Ship.h"
class EnemyShip : public Ship
{
public:
	EnemyShip();
	void Shoot();
	void setTexture(const sf::Texture& texture);
private:
	void Update(float deltaTime) = 0;

	Texture enemyTex;
	Texture enemyTex2;
	Texture enemyTex3;
	Sprite enemySpr;
};

