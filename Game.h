#pragma once

class Game
{
public:
	Game();
	~Game();

	void Init(struct SDL_Window* window);

	void Run();

private:
	bool m_IsRunning = true;
};