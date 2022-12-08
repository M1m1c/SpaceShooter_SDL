#pragma once
#include <string>
#include <memory>
#include <array>
#include "ECSCore.h"

class ECSRegistry;
class Entity;
class ISystem;
class PlayerController;
class ThrottleSystem;
class RenderSystem;
class MoveTranslateSystem;
class WeaponSystem;

const uint32_t MAX_SYSTEMS = 32;

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
	template<typename T,typename... Args>
	void AddSystem(Args&&... args);

	bool m_IsRunning = true;
	float m_LastFrameTime = 0.f;

	int m_Width=0;
	int m_Height=0;

	struct SDL_Window* m_Window;
	struct SDL_Surface* m_Surface;
	struct SDL_Renderer* m_Renderer;
	std::shared_ptr<union SDL_Event> m_EventHandle;

	std::shared_ptr<ECSRegistry> m_ECSRegistry;

	uint32_t m_SystemCount = 0;
	std::array<std::unique_ptr<ISystem>, MAX_SYSTEMS> m_Systems;

	std::unique_ptr<ThrottleSystem> m_ThrottleSystem;
	std::unique_ptr<RenderSystem> m_RenderSystem;
	std::unique_ptr<MoveTranslateSystem> m_MoveTranslateSystem;
	std::unique_ptr<WeaponSystem> m_WeaponSystem;

	EntityID playerEntity;
};


