#include "PlayerShip.h"
#include <SFML/Window/Keyboard.hpp>

PlayerShip::PlayerShip() : Ship()
{
	setShipType(ShipType::PlayerShip);
	speed = 400.0f;
}

void PlayerShip::Update(float deltaTime)
{
	//if (!IsVisible) return;
	Vector2f pos = shipSpr.getPosition();

	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{
		pos.x -= speed * deltaTime;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{
		pos.x += speed * deltaTime;
	}

	float boundsOffset = 31.5f;

	if (pos.x < boundsOffset)
	{
		pos.x = boundsOffset;
	}
	else if (pos.x > 768.0f - boundsOffset)
	{
		pos.x = 768.0f - boundsOffset;
	}
	shipSpr.setPosition(pos);
}