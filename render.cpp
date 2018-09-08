#include "common.h"
#include "render.h"

void Renderer::beginRender(Vector topLeft, int width, int height, TransformMatrix transform, double scaleFactor) {
	// start drawing to a hidden backbuffer - this
	// backbuffer will be presented when we call
	// SDL_RenderPresent
	SDL_SetRenderDrawColor(SDLRenderer, 0, 0, 0, 0);
	SDL_RenderClear(SDLRenderer);

	// width and height may differ from the screenspace
	// this represents 'zoom' and must be accoutned for
	// as a scaling factor for both entity sizes and 
	// screenspace coordinates
	currentTopLeft.x = topLeft.x;
	currentTopLeft.y = topLeft.y;
	currentWidth = width;
	currentHeight = height;
	currentTransform = transform;
	currentScaleFactor = scaleFactor;
}

void Renderer::endRender() {
	SDL_RenderPresent(SDLRenderer);
}

void Renderer::drawStars(std::vector<Vector>& stars) {
	SDL_SetRenderDrawColor(SDLRenderer, 255, 255, 255, 255);
	for (auto& star : stars) {
		// stars are not transformed
		Vector viewportPosition = computeScreenspaceCoordinates(star, 0.01, false);
		SDL_RenderDrawPoint(SDLRenderer, viewportPosition.x, viewportPosition.y);
	}
}

void Renderer::drawEntities(std::vector<Entity*>& entities) {
	for (auto& entity : entities) {
		drawEntity(entity);
	}
}

void Renderer::drawEntity(Entity* entity) {
	if (!entity->_virtual) {
		SDL_Texture* texture = textureFromBMP(entity->assetFile);
		if (texture != nullptr) {
			// I don't know what this is
			SDL_Rect source;
			source.x = 0;
			source.y = 0;
			source.w = entity->assetFileWidth; // planet->width;
			source.h = entity->assetFileHeight; // planet->height;

			// the view coordinates are potentially offset by the
			// moving/scaling viewport, so use transformed coords
			SDL_Rect planetBox;
			Vector screenPosition = computeScreenspaceCoordinates(entity->position, 1.0);
			planetBox.x = screenPosition.x;
			planetBox.y = screenPosition.y;
			planetBox.w = entity->width * (1.0 / currentScaleFactor);
			planetBox.h = entity->height * (1.0 / currentScaleFactor);
			SDL_RenderCopy(SDLRenderer, texture, &source, &planetBox);
		}
	}
}

SDL_Texture* Renderer::textureFromBMP(std::string fileName) {
	SDL_Texture* cached = textureCache[fileName];
	if (cached != nullptr) {
		return cached;
	}

	SDL_Surface* bmp = SDL_LoadBMP(fileName.c_str());
	if (bmp == nullptr) {
		return nullptr;
	}
	// set white to transparent
	long white = SDL_MapRGB(bmp->format, 255, 255, 255);
	SDL_SetColorKey(bmp, SDL_TRUE, white);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(SDLRenderer, bmp);
	cacheTexture(fileName, texture);
	SDL_FreeSurface(bmp);
	return texture;
}

bool Renderer::isEntityInViewport(Entity* entity) {
	// todo 2d frustum cull - does SDL do this already?
	return true;
}

Vector Renderer::computeScreenspaceCoordinates(Vector gamePosition, float parallaxCoefficient, bool applyTransform) {
	// converts the entity's gameworld coordinates to the constrained screensurface (1024x768 0,0 topleft)
	Vector result;

	// gamespace coordinates are offset by the viewport coordinates w/zoom transform
	result.x = gamePosition.x - currentTopLeft.x * parallaxCoefficient;
	result.y = gamePosition.y - currentTopLeft.y * parallaxCoefficient;

	// transform since the viewport might be larger than 1024x768
	// transform here always describes how to get from the viewport coordinates
	// to the screenspace 1024x768 coordinates and is updated each tick
	if (applyTransform) {
		result = result.applyTransform(currentTransform);
	}

	return result;
}

void Renderer::cacheTexture(std::string fileName, SDL_Texture* texture) {
	textureCache[fileName] = texture;
}

void Renderer::destroyTextureCache() {
	for (auto& kv : textureCache) {
		SDL_DestroyTexture(kv.second);
	}
}
