#include "SDL.h"
#include "Game.h"

Game game;
bool IsRunning = true;

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("SpaceShooter_SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_SHOWN);

	game.Init(window);

	game.Run();
	

	return 0;
}