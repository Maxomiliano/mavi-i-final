#pragma once
#include "Projectile.h"

class EnemyProjectile : public Projectile
{
public:
	EnemyProjectile(Vector2f startPos, const Texture& tex);
	void Update(float deltaTime) override;

private:
	float acceleration;
};

