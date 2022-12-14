#pragma once
#include <string>
#include <memory>
#include <array>
#include <queue>
#include "ECS/ECSCore.h"
#include "containers/FixedQueue.h"
#include "containers/QuadTree.h"
#include "systems/SpawnOrder.h"

class ECSRegistry;
class Entity;
class ISystem;
class PlayerController;
class ThrottleSystem;
class DrawSystem;
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

	std::shared_ptr<QuadTree<EntityID>> m_QuadTree;

	uint32_t m_SystemCount = 0;
	std::array<std::unique_ptr<ISystem>, MAX_SYSTEMS> m_Systems;

	FixedQueue<SpawnOrder,MAX_ENTITIES> m_SpawnOrders;

	EntityID playerEntity;
};


