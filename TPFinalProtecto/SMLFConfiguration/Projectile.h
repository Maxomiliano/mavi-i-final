#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Projectile
{
public:
	Projectile(Vector2f startPos, const Texture& tex);
	virtual ~Projectile();

	virtual void Update(float deltatime) = 0;

	void Draw(RenderWindow& window);

	bool IsOutOfBounds() const;
	FloatRect getBounds() const;

protected:
	Sprite projectileSpr;
	float speed;
};

