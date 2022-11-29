#include "Game.h"

#include "SDL.h"
#include "Scene.h"


Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(SDL_Window* window, SDL_Surface* surface)
{
	m_Window = window;
	m_Surface = surface;
	m_ActiveScene = std::make_unique<Scene>();
}

void Game::Run()
{
	while (m_IsRunning) 
	{	
		float currentTime = SDL_GetTicks64();
		float deltaTime = currentTime - m_LastFrameTime;
		m_LastFrameTime = currentTime;

		SDL_FillRect(m_Surface, NULL, SDL_MapRGB(m_Surface->format, 20, 20, 30));
		SDL_UpdateWindowSurface(m_Window);

		m_ActiveScene->OnUpdate(deltaTime);
	}
}
