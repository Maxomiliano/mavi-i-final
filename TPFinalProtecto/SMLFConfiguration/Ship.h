#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

class Ship
{
public: Ship();
	  enum class ShipType
	  {
		  EnemyShip,
		  PlayerShip
	  };
	  virtual ~Ship();
	  virtual void Spawn(const Vector2f& position);
	  virtual void Draw(RenderWindow& window);
	  virtual void Update(float deltaTime) = 0;

	  bool IsVisible() const;
	  void setTexture(const Texture& texture);
	  void setScale(float x, float y);
	  void setOrigin(float x, float y);
	  void setShipType(ShipType shipType) { type = shipType; }
	  ShipType getShipType() { return type; }
protected:
	ShipType type;
	Sprite shipSpr;
	bool isVisible;
	float speed;
};