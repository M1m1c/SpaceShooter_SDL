#pragma once

class Game
{
public:
	Game();
	~Game();

	void Init(struct SDL_Window* window, struct SDL_Surface* surface);

	void Run();

private:
	bool m_IsRunning = true;

	struct SDL_Window* m_Window;
	struct SDL_Surface* m_Surface;
};