#pragma once

#include "entity.h"

class Player : public Entity {
public:
	Player() {
		_virtual = true;
	}
};
