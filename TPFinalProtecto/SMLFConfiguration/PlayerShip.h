#pragma once
#include "Ship.h"

class PlayerShip : public Ship
{
public:
	PlayerShip();
	void Update(float deltaTime) override;
};

