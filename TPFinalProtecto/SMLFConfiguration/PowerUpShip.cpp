#include "PowerUpShip.h"

PowerUpShip::PowerUpShip() : EnemyShip()
{
	setShipType(ShipType::PowerUpShip);

	speed = 150;
	hitPoints = 1;
	shootCooldown = 0.7f;
}