#include "Character.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
using namespace sf;

Character::Character()
{
	isVisible = true;
	std::srand(static_cast<unsigned int>(std::time(NULL)));
}

Character::~Character()
{

}

void Character::Spawn(Vector2f& position)
{
	charSpr.setPosition(position);
	isVisible = true;
}

bool Character::IsClicked(Vector2f crossPosition)
{
	if (isVisible && charSpr.getGlobalBounds().contains(crossPosition))
	{
		isVisible = false;
		return true;
	}
	return false;
}

void Character::Draw(RenderWindow& window)
{
	if (isVisible)
	{
		window.draw(charSpr);
	}
}

bool Character::IsVisible() const
{
	return isVisible;
}

void Character::setTexture(const sf::Texture& texture) {
	charSpr.setTexture(texture);
}

void Character::setScale(float x, float y) {
	charSpr.setScale(x, y);
}

void Character::setOrigin(float x, float y)
{
	charSpr.setOrigin(x, y);
}
