#pragma once
#include <memory>
#include "Entity.h"
class Scene;

class Game
{
public:
	Game();
	~Game();

	void Init(struct SDL_Window* window, struct SDL_Surface* surface);

	void Run();

private:
	bool m_IsRunning = true;
	float m_LastFrameTime = 0.f;

	struct SDL_Window* m_Window;
	struct SDL_Surface* m_Surface;

	std::unique_ptr<Scene> m_ActiveScene;
	Entity testEntity;
};