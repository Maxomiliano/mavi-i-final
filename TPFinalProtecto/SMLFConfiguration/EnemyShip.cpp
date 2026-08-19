#include "EnemyShip.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

EnemyShip::EnemyShip() : Ship()
{
	setShipType(ShipType::EnemyShip);
	speed = 150.0f;
	moveDirection = 1.0f;
}

/*
void EnemyShip::Shoot()
{

}
*/

void EnemyShip::Update(float deltaTime)
{
	if (!isVisible) return;
	Vector2f pos = shipSpr.getPosition();

	pos.x += speed * moveDirection * deltaTime;

	float boundsOffset = 30.0f;
	if (pos.x > 768.0f - boundsOffset)
	{
		pos.x = 768.0f - boundsOffset;
		moveDirection = -1.0f;
	}
	else if (pos.x < boundsOffset)
	{
		pos.x = boundsOffset;
		moveDirection = 1.0f;
	}

	shipSpr.setPosition(pos);
}

/*
void EnemyShip::setTexture(const Texture& texture) {
	shipSpr.setTexture(texture);
}
*/
