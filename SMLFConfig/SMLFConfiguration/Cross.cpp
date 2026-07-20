#include "Cross.h"

Cross::Cross()
{
	crossSpr.setOrigin(64, 64);
}

void Cross::UpdatePosition(Vector2f position)
{
	crossSpr.setPosition(position);
}

void Cross::Draw(RenderWindow& window)
{
	window.draw(crossSpr);
}

void Cross::setTexture(const sf::Texture& texture) {
	crossSpr.setTexture(texture);
}