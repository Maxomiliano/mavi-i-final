#include "Enemy.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

Enemy::Enemy() : Character()
{
	setCharacterType(CharacterType::Enemy);
}

void Enemy::Shoot()
{

}


void Enemy::setTexture(const Texture& texture) {
	charSpr.setTexture(texture);
}
