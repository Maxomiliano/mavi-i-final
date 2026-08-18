#include "EnemyProjectile.h"

EnemyProjectile::EnemyProjectile(Vector2f startPos, const Texture& tex) : Projectile(startPos, tex)
{
	speed = 50.0f;
	acceleration = 400.0f;
}

void EnemyProjectile::Update(float deltaTime)
{
	Vector2f pos = projectileSpr.getPosition();

	speed += acceleration * deltaTime;
	pos.y += speed * deltaTime;
	projectileSpr.setPosition(pos);
}