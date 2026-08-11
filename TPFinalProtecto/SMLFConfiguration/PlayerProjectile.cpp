#include "PlayerProjectile.h"

PlayerProjectile::PlayerProjectile(Vector2f startPos, const Texture& tex) : Projectile(startPos, tex)
{
	speed = 600.0f;
}

void PlayerProjectile::Update(float deltaTime)
{
	Vector2f pos = projectileSpr.getPosition();

	//MRU: Xf = Xi + V * t
	pos.y -= speed * deltaTime;

	projectileSpr.setPosition(pos);
}