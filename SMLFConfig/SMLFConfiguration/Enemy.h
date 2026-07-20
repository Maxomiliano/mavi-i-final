#pragma once
#include "Character.h"
class Enemy : public Character
{
public:
	Enemy();
	void Shoot();
	void setTexture(const sf::Texture& texture);
private:
	Texture enemyTex;
	Texture enemyTex2;
	Texture enemyTex3;
	Sprite enemySpr;
};

