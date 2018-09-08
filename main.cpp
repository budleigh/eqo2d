#include <Windows.h>
#include <iostream>
#include <vector>

#include <SDL.h>

#include "common.h"
#include "game.h"

void sdlError(void) {
	std::cout << "SDL error: " << SDL_GetError() << std::endl;
}

void initialize(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		sdlError();
	}
}

void launchDebugTerminal(void) {
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
}

SDL_Window* createSDLWindow(void) {
	SDL_Window* window = SDL_CreateWindow(
		"eqo", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN
	);

	if (window == nullptr) {
		sdlError();
	}

	return window;
}

SDL_Renderer* createSDLRenderer(SDL_Window* window) {
	SDL_Renderer* SDLRenderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED
	);

	if (SDLRenderer == nullptr) {
		sdlError();
	}

	return SDLRenderer;
}

void sdlShutdown(SDL_Renderer* SDLRenderer, SDL_Window* window) {
	SDL_DestroyRenderer(SDLRenderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void gameShutdown(Game* game) {
	delete game;
}

void mainLoop(SDL_Renderer* SDLRenderer) {
#ifdef _DEBUG
	// doesn't seem to work all the time for some reason
	// wonder if SDL yoinks stdout for something
	launchDebugTerminal();
#endif

	Game* game = new Game(SDLRenderer);

	bool running = true;
	SDL_Event event;
	std::vector<SDL_Event> eventList;

	while (running) {
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			default:
				eventList.push_back(event);
			}
		}

		if (running) {
			running = game->update(eventList);
			game->render();
		}
	}

	gameShutdown(game);
}

int main(int argc, char *argv[]) {
	SDL_Window* SDLWindow = createSDLWindow();
	SDL_Renderer* SDLRenderer = createSDLRenderer(SDLWindow);
	mainLoop(SDLRenderer);
	sdlShutdown(SDLRenderer, SDLWindow);

	return 0;
}
