#pragma once

#include "entity.h"

struct ShipCost {
	int gas;
	int mineral;
};

class Ship : public Entity {
public:
	int health;
};

class Transport : public Ship {
public:
	static const struct ShipCost cost;
};
