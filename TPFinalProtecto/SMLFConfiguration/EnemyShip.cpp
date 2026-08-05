#include "EnemyShip.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

EnemyShip::EnemyShip() : Ship()
{
	setShipType(ShipType::EnemyShip);
}

void EnemyShip::Shoot()
{

}

void EnemyShip::Update(float deltaTime)
{

}


void EnemyShip::setTexture(const Texture& texture) {
	shipSpr.setTexture(texture);
}
