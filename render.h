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
	Renderer(SDL_Renderer* SDLRenderer) : SDLRenderer(SDLRenderer) {}
	~Renderer() {
		destroyTextureCache();
	}

	// beginrender takes the current viewport properties
	void beginRender(Vector topLeft, int width, int height, TransformMatrix transform, double scaleFactor);
	void endRender();
	void drawStars(std::vector<Vector>& stars);
	void drawEntities(std::vector<Entity*>& entities);
	void drawEntity(Entity* entity);

private:
	// viewport dimensions and transform, tracked by tick
	Vector currentTopLeft;
	double currentWidth;
	double currentHeight;
	double currentScaleFactor;
	TransformMatrix currentTransform;

	SDL_Renderer* SDLRenderer;
	std::map<std::string, SDL_Texture*> textureCache;

	// viewport utilities
	bool isEntityInViewport(Entity* entity);
	Vector computeScreenspaceCoordinates(Vector position, float parallaxCoefficient, bool applyTransform = true);

	// asset loading and destroying
	SDL_Texture* textureFromBMP(std::string filename);
	void cacheTexture(std::string fileName, SDL_Texture* texture);
	void destroyTextureCache();
};
