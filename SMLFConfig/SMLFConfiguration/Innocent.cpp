#include "Innocent.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

Innocent::Innocent() : Character()
{
	setCharacterType(CharacterType::Innocent);
}

void Innocent::DissapearAfterTime()
{
	isVisible = false;
}


void Innocent::setTexture(const Texture& texture) {
	charSpr.setTexture(texture);
}

