#pragma once
#include "Ship.h"

class EnemyShip : public Ship
{
public:
	EnemyShip();
	//void Shoot();
	//void setTexture(const sf::Texture& texture);
	void Update(float deltaTime) override;
	void setDirection(float dir);
	void TakeDamage(int damage);
	bool IsDead() const;
	bool CanShoot(float deltaTime);

private:

	/*
	Texture enemyTex;
	Texture enemyTex2;
	Texture enemyTex3;
	Sprite enemySpr;
	*/

protected:

	float moveDirection;
	float speed;
	int hitPoints;
	float shootTimer;
	float shootCooldown;
};

