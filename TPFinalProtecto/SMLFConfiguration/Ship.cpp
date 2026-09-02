#include "Ship.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
using namespace sf;

Ship::Ship()
{
	isVisible = true;
	speed = 0.0;
	std::srand(static_cast<unsigned int>(std::time(NULL)));
}

Ship::~Ship()
{

}

void Ship::Spawn(const Vector2f& position)
{
	shipSpr.setPosition(position);
	isVisible = true;
}

void Ship::Draw(RenderWindow& window)
{
	if (isVisible)
	{
		window.draw(shipSpr);
	}
}

void Ship::Update(float deltaTime)
{

}

bool Ship::IsVisible() const
{
	return isVisible;
}

void Ship::setTexture(const sf::Texture& texture) {
	shipSpr.setTexture(texture);
}

void Ship::setScale(float x, float y) {
	shipSpr.setScale(x, y);
}

void Ship::setOrigin(float x, float y)
{
	shipSpr.setOrigin(x, y);
}

Vector2f Ship::getPosition() const
{
	return shipSpr.getPosition();
}

FloatRect Ship::getBounds() const
{
	return shipSpr.getGlobalBounds();
}
