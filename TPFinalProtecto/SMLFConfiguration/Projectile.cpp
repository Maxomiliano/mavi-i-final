#include "Projectile.h"

Projectile::Projectile(Vector2f startPos, const Texture& tex)
{
	projectileSpr.setTexture(tex);
	projectileSpr.setPosition(startPos);

	FloatRect bounds = projectileSpr.getLocalBounds();
	projectileSpr.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
}

Projectile::~Projectile()
{
}

void Projectile::Draw(RenderWindow& window)
{
	window.draw(projectileSpr);
}

bool Projectile::IsOutOfBounds() const
{
	Vector2f pos = projectileSpr.getPosition();
	return (pos.y < -50.0f || pos.y > 1074.0f);
}

FloatRect Projectile::getBounds() const
{
	return projectileSpr.getGlobalBounds();
}