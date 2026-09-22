#include "FastEnemyShip.h"

FastEnemyShip::FastEnemyShip() : EnemyShip()
{
	speed = 250.0f;
	hitPoints = 1;
	shootCooldown = 0.5f;
}