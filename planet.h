#pragma once

#include <string>
#include <json.h>

#include "entity.h"

using nlohmann::json;

enum PlanetType {
	rock,
	gas
};

class Planet : public Entity {
public:
	std::string name;
	long radius;
	PlanetType type;
	ID playerOwner;

	void loadJson(json planetJson);
};

class Base : public Entity {
public:
	long numWorkers = 1;
	long minerals = 0;
	long gases = 0;
	ID planet;
};
