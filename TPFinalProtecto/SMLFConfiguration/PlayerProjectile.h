#pragma once
#include "Projectile.h"

class PlayerProjectile : public Projectile
{
public:
	PlayerProjectile(Vector2f startPos, const Texture& tex);

	void Update(float deltaTime) override;
};

