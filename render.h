#pragma once

#include <vector>

#include <SDL.h>

#include "entity.h"
#include "math.h"
#include "planet.h"
#include "ship.h"
#include "viewport.h"

class Renderer {
public:
	Renderer() {}
	Renderer(SDL_Renderer* SDLRenderer, Viewport* viewport) : SDLRenderer(SDLRenderer), viewport(viewport) {}
	~Renderer() {
		destroyTextureCache();
	}

	Viewport* viewport;

	// beginrender takes the current viewport properties
	void beginRender();
	void endRender();
	void drawStars(std::vector<Vector>& stars);
	void drawEntities(std::vector<Entity*>& entities);
	void drawEntity(Entity* entity);

private:
	SDL_Renderer* SDLRenderer;
	std::map<std::string, SDL_Texture*> textureCache;

	// asset loading and destroying
	SDL_Texture* textureFromBMP(std::string filename);
	void cacheTexture(std::string fileName, SDL_Texture* texture);
	void destroyTextureCache();
};
