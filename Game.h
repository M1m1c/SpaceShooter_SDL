#pragma once
#include <string>
#include <memory>

class ECSRegistry;
class Entity;
class PlayerController;
class MovementSystem;
class RenderSystem;
class MoveTranslateSystem;

class Game
{
public:
	Game();
	~Game();

	void Init(struct SDL_Window* window, struct SDL_Surface* surface, const int width, const int height);

	void Run();

	std::shared_ptr<ECSRegistry> GetECSRegistry();

	Entity& CreateEntity(const std::string& name = std::string());

private:
	bool m_IsRunning = true;
	float m_LastFrameTime = 0.f;

	int m_Width=0;
	int m_Height=0;

	struct SDL_Window* m_Window;
	struct SDL_Surface* m_Surface;
	struct SDL_Renderer* m_Renderer;
	std::shared_ptr<union SDL_Event> m_EventHandle;

	std::shared_ptr<ECSRegistry> m_ECSRegistry;

	std::unique_ptr<PlayerController> m_PlayerController;
	std::unique_ptr<MovementSystem> m_MovementSystem;
	std::unique_ptr<RenderSystem> m_RenderSystem;
	std::unique_ptr<MoveTranslateSystem> m_MoveTranslateSystem;

	Entity* playerEntity;
};