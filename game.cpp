#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>

#include <SDL.h>
#include <json.h>

#include "common.h"
#include "game.h"

using nlohmann::json;

void Game::initialize() {
	loadPlanetMap();
	generateStarMap();
}

void Game::loadPlanetMap() {
	std::ifstream stream("planets.json");
	json planetsJson = json::parse(stream);
	
	for (auto& planetJson : planetsJson) {
		Planet* planet = createPlanet();
		planet->loadJson(planetJson);
	}
}

void Game::generateStarMap() {
	srand(time(NULL));

	// because of parallax, we need extra stars beyond the world boundaries visually
	long starBoundaryLeft = WORLD_BOUNDARY_LEFTX - 1000;
	long starBoundaryRight = WORLD_BOUNDARY_RIGHTX + 1000;
	long starBoundaryUp = WORLD_BOUNDARY_UPY - 1000;
	long starBoundaryDown = WORLD_BOUNDARY_DOWNY + 1000;

	float starDistribution = .0008;

	for (int x = 0; x < (WORLD_WIDTH * WORLD_HEIGHT) * starDistribution; x++) {
		Vector star;
		star.x = starBoundaryLeft + (rand() % static_cast<int>(starBoundaryRight - starBoundaryLeft) + 1);
		star.y = starBoundaryUp + (rand() % static_cast<int>(starBoundaryDown - starBoundaryUp) + 1);
		stars.push_back(star);
	}
}

void Game::update(std::vector<SDL_Event>& eventList) {
	// viewport update reacts to input events
	viewport.update(eventList);

	// update all game entities
	for (auto& entity : entities) {
		entity->update(eventList, *this);
	}
}

void Game::render() {
	// signal to start filling backbuffer
	renderer->beginRender(viewport.topLeft, viewport.width, viewport.height, viewport.transform, viewport.scaleFactor);

	// order here matters!
	renderer->drawStars(stars);
	renderer->drawEntities(entities);

	// flip the swapchain
	renderer->endRender();
}

Entity* Game::getEntityById(const char* id) {
	return entitiesById[id];
}

Player* Game::createPlayer() {
	Player* player = new Player();
	entitiesById.insert(std::make_pair(player->id, player));
	entities.push_back(player);
	return player;
}

Ship* Game::createShip() {
	Ship* ship = new Ship();
	ship->assetFile = "assets/ships/ship.bmp";
	ship->assetFileWidth = 2048;
	ship->assetFileHeight = 2742;
	entitiesById.insert(std::make_pair(ship->id, ship));
	entities.push_back(ship);
	return ship;
}

Planet* Game::createPlanet() {
	Planet* planet = new Planet();
	planet->assetFileWidth = 2048;
	planet->assetFileHeight = 2048;
	entitiesById.insert(std::make_pair(planet->id, planet));
	entities.push_back(planet);
	return planet;
}

void Game::destroyEntities() {
	for (auto& entity : entities) {
		delete entity;
	}

	entities.clear();
	entitiesById.clear();
}

void Game::destroyEntity(const char* id) {
	Entity* entity = getEntityById(id);
	if (entity != nullptr) {
		delete entity;
	}

	entitiesById.erase(id);

	for (std::vector<Ship*>::size_type x = 0; x != entities.size(); x++) {
		if (entities[x]->id == id) {
			entities.erase(entities.begin() + x, entities.begin() + x);
		}
	}
}

std::string Game::serializeState() {
	return "fuck";
}
