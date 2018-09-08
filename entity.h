#pragma once

#include <vector>

#include <SDL.h>

#include "util.h"
#include "math.h"

typedef char* ID;

class Game;

class Entity {
public:
	Entity() {
		id = createUUID();
	}

	// virtuals are not rendered
	ID id;
	bool _virtual;

	// asset properties
	std::string assetFile;
	int assetFileWidth;
	int assetFileHeight;

	// generic existential properties
	Vector position;
	// screenPosition is the transformed position relative to the viewport
	Vector screenPosition;
	int width;
	int height;

	// generic gameplay flags
	bool selected;
	bool alive;

	// entities can access all SDL_Events as well as the Game's public
	// entity API to interogate things like location and other entity
	// state.
	virtual void update(std::vector<SDL_Event> eventList, Game& gameStateAPI) {};

	// generic interaction helpers
	bool mouseOver();
};
