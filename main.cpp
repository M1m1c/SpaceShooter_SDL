#include <stdio.h>
#include "SDL.h"
#include "Game.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

Game game;

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(
		"SpaceShooter_SDL",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
	
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
	
	game.Init(window, screenSurface);

	game.Run();



	SDL_Quit();
	return 0;
}