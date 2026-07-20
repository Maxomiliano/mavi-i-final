#pragma once
#include "Character.h"
class Innocent : public Character
{
public:
	Innocent();
	void DissapearAfterTime();
	void setTexture(const sf::Texture& texture);
private:
	Texture inoTex;
	Sprite inoSpr;
};