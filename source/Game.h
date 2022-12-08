#pragma once
#include <string>
#include <memory>
#include "ECSCore.h"

class ECSRegistry;
class Entity;
class PlayerController;
class ThrottleSystem;
class RenderSystem;
class MoveTranslateSystem;
class WeaponSystem;

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
	std::unique_ptr<ThrottleSystem> m_ThrottleSystem;
	std::unique_ptr<RenderSystem> m_RenderSystem;
	std::unique_ptr<MoveTranslateSystem> m_MoveTranslateSystem;
	std::unique_ptr<WeaponSystem> m_WeaponSystem;

	EntityID playerEntity;
};