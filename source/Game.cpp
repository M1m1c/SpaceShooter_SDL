#include "Game.h"

#include "SDL.h"
#include "ECSRegistry.h"
#include "Components.h"
#include "ComponentView.h"
#include "systems/ISystem.h"
#include "systems/PlayerController.h"
#include "systems/ThrottleSystem.h"
#include "systems/RenderSystem.h"
#include "systems/MoveTranslateSystem.h"
#include "systems/WeaponSystem.h"
#include "systems/DestructionSystem.h"
#include "systems/EnemySpawnerSystem.h"
#include "systems/EntitySpawnSystem.h"
#include <iostream>
#include <memory>

const float MILLI_TO_SECONDS = 0.001f;

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(SDL_Window* window, SDL_Surface* surface, const int width, const int height)
{
	m_Width = width;
	m_Height = height;

	m_Window = window;
	m_Surface = surface;
	m_Renderer = SDL_CreateRenderer(window, -1, 0);
	m_EventHandle = std::make_shared<SDL_Event>();

	m_ECSRegistry = std::make_shared<ECSRegistry>();
	m_ECSRegistry->Init();

	//Registering components with ECS registry
	m_ECSRegistry->RegisterComponent<TransformComp>();
	m_ECSRegistry->RegisterComponent<InputComp>();
	m_ECSRegistry->RegisterComponent<TagComp>();
	m_ECSRegistry->RegisterComponent<RigidBodyComp>();
	m_ECSRegistry->RegisterComponent<WeaponComp>();
	m_ECSRegistry->RegisterComponent<HealthComp>();


	playerEntity = m_ECSRegistry->CreateEntity<InputComp, RigidBodyComp, WeaponComp>(
		Vector4(640.f, 400.f, 15.f, 15.f),
		ObjectTag::Player);


	

	auto& inputComp = m_ECSRegistry->GetComponent<InputComp>(playerEntity);

	//Systems added in execution order
	AddSystem<PlayerController>(m_EventHandle, inputComp);
	AddSystem<ThrottleSystem>(m_ECSRegistry->CreateComponentView<RigidBodyComp,InputComp>());
	AddSystem<RenderSystem>(m_Renderer, m_ECSRegistry->CreateComponentView<TransformComp, TagComp>());
	AddSystem<WeaponSystem>(m_SpawnOrders, m_ECSRegistry->CreateComponentView<TransformComp, InputComp, TagComp, WeaponComp>());
	AddSystem<MoveTranslateSystem>(m_Renderer, m_Width, m_Height, m_ECSRegistry->CreateComponentView<TransformComp, RigidBodyComp, TagComp, HealthComp>());
	AddSystem<EnemySpawnerSystem>(m_ECSRegistry);
	AddSystem<EntitySpawnSystem>(m_ECSRegistry, m_SpawnOrders);
	AddSystem<DestructionSystem>(m_ECSRegistry);

	//TODO pass ref to spawnorder queue for things that need to spawn something;
}

void Game::Run()
{
	while (m_IsRunning)
	{
		float currentTime = SDL_GetTicks64();
		float deltaTime = (currentTime - m_LastFrameTime) * MILLI_TO_SECONDS;
		m_LastFrameTime = currentTime;

		SDL_SetRenderDrawColor(m_Renderer, 20, 20, 30, 255);
		SDL_RenderClear(m_Renderer);

		//TODO maybe we should just get all the components of active enties here at once and tehn filter them down to the systems,
		// this would avoid having to read for each system

		for (size_t i = 0; i < m_SystemCount; i++)
		{
			m_Systems[i]->Update(deltaTime);
		}

		SDL_RenderPresent(m_Renderer);
	}
}

std::shared_ptr<ECSRegistry> Game::GetECSRegistry()
{
	return m_ECSRegistry;
}

template<typename T, typename ...Args>
void Game::AddSystem(Args && ...args)
{
	m_Systems[m_SystemCount] = ISystem::Create<T>(args...);
	m_SystemCount++;
}