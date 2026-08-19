#pragma once
#include "Ship.h"
class EnemyShip : public Ship
{
public:
	EnemyShip();
	//void Shoot();
	//void setTexture(const sf::Texture& texture);
	void Update(float deltaTime) override;

private:

	Texture enemyTex;
	Texture enemyTex2;
	Texture enemyTex3;
	Sprite enemySpr;

	float moveDirection;
	float speed;
};

