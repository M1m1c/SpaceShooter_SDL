#pragma once
#include <string>
#include <memory>
//class Game;
class ECSRegistry;
class Entity;

class Game
{
public:
	Game();
	~Game();

	void Init(struct SDL_Window* window, struct SDL_Surface* surface);

	void Run();

	std::shared_ptr<ECSRegistry> GetECSRegistry();

	Entity& CreateEntity(const std::string& name = std::string());

private:
	bool m_IsRunning = true;
	float m_LastFrameTime = 0.f;

	struct SDL_Window* m_Window;
	struct SDL_Surface* m_Surface;
	struct SDL_Renderer* m_Renderer;

	std::shared_ptr<ECSRegistry> m_ECSRegistry;
	Entity* testEntity;

	//std::unique_ptr<Game> m_ActiveGame;
};