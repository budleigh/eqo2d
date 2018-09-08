#pragma once

#include <map>
#include <vector>

#include <SDL.h>

#include "common.h"
#include "math.h"
#include "entity.h"
#include "player.h"
#include "planet.h"
#include "render.h"
#include "viewport.h"

class Game {
public:
	Game() {}

	Game(SDL_Renderer* sdlRenderer) {
		player1 = createPlayer();
		initialize();

		renderer = new Renderer(sdlRenderer, &viewport);
	}

	~Game() {
		destroyEntities();
		delete renderer;
	}
	// lifecycle and mainloop
	void initialize();
	bool update(std::vector<SDL_Event>& eventList);
	void render();

	// game state API
	Entity* getEntityById(const char* id);
	void destroyEntity(const char* id);

private:
	// rendering concerns
	Viewport viewport;
	Renderer* renderer;

	// state concerns
	Player* player1;
	std::vector<Vector> stars;
	std::vector<Entity*> entities;
	std::map<const char*, Entity*> entitiesById;

	// internal - state initialization
	void loadPlanetMap();
	void generateStarMap();
	Player* createPlayer();
	Ship* createShip();
	Planet* createPlanet();

	// entity management
	void destroyEntities();

	// multiplayer concerns - serialization
	std::string serializeState();
};
