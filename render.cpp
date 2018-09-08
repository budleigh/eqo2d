#include "common.h"
#include "render.h"

void Renderer::beginRender() {
	// start drawing to a hidden backbuffer - this
	// backbuffer will be presented when we call
	// SDL_RenderPresent
	SDL_SetRenderDrawColor(SDLRenderer, 0, 0, 0, 0);
	SDL_RenderClear(SDLRenderer);
}

void Renderer::endRender() {
	SDL_RenderPresent(SDLRenderer);
}

void Renderer::drawStars(std::vector<Vector>& stars) {
	SDL_SetRenderDrawColor(SDLRenderer, 255, 255, 255, 255);
	for (auto& star : stars) {
		// stars are not transformed
		Vector viewportPosition = viewport->computeScreenspaceCoordinates(star, 0.01, false);
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
			planetBox.x = entity->screenPosition.x;
			planetBox.y = entity->screenPosition.y;
			planetBox.w = entity->width * (1.0 / viewport->scaleFactor);
			planetBox.h = entity->height * (1.0 / viewport->scaleFactor);
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

void Renderer::cacheTexture(std::string fileName, SDL_Texture* texture) {
	textureCache[fileName] = texture;
}

void Renderer::destroyTextureCache() {
	for (auto& kv : textureCache) {
		SDL_DestroyTexture(kv.second);
	}
}
