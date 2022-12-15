#include "Game.h"

#include "SDL.h"
#include "ECSRegistry.h"
#include "Components.h"
#include "SystemView.h"
#include "systems/ISystem.h"
#include "systems/PlayerController.h"
#include "systems/ThrottleSystem.h"
#include "systems/RenderSystem.h"
#include "systems/MoveCalcSystem.h"
#include "systems/MoveTranslateSystem.h"
#include "systems/BorderSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/WeaponSystem.h"
#include "systems/DestructionSystem.h"
#include "systems/WaveSpawnerSystem.h"
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


	//Creating systems views
	auto throttleView = m_ECSRegistry->CreateComponentView<RigidBodyComp, InputComp>();
	auto moveView = m_ECSRegistry->CreateComponentView<TransformComp, RigidBodyComp>();
	auto renderView = m_ECSRegistry->CreateComponentView<TransformComp, TagComp>();
	auto weaponView = m_ECSRegistry->CreateComponentView<TransformComp, InputComp, TagComp, WeaponComp>();
	auto collisionView = m_ECSRegistry->CreateComponentView<TransformComp, TagComp, HealthComp>();

	m_ECSRegistry->SetThrottleView(throttleView);
	m_ECSRegistry->SetMoveView(moveView);
	m_ECSRegistry->SetRenderView(renderView);
	m_ECSRegistry->SetWeaponView(weaponView);
	m_ECSRegistry->SetCollisionView(collisionView);


	//Creating Player
	playerEntity = m_ECSRegistry->CreateEntity<InputComp, RigidBodyComp, WeaponComp>(
		Vector4(640.f, 400.f, 15.f, 15.f),
		ObjectTag::Player);
	auto& inputComp = m_ECSRegistry->GetComponent<InputComp>(playerEntity);


	//Systems added in execution order
	AddSystem<PlayerController>(m_EventHandle, inputComp);
	AddSystem<ThrottleSystem>(throttleView);
	AddSystem<RenderSystem>(m_Renderer, renderView);
	AddSystem<WeaponSystem>(m_SpawnOrders, weaponView);
	AddSystem<MoveCalcSystem>(moveView);
	AddSystem<BorderSystem>(m_Width,m_Height,collisionView);
	AddSystem<CollisionSystem>(m_Renderer,collisionView);
	AddSystem<MoveTranslateSystem>(moveView);
	AddSystem<WaveSpawnerSystem>(m_SpawnOrders);
	AddSystem<EntitySpawnSystem>(m_ECSRegistry, m_SpawnOrders);
	AddSystem<DestructionSystem>(m_ECSRegistry);
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

		//TODO maybe we should just get all the components of active enties here at once and them filter them down to the systems,
		// this would avoid having to read for each system,
		// make class with an array that holds pointers to the componentviews and sets their tuples at the start here



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