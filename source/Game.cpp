#include "Game.h"

#include "SDL.h"
#include "ECSRegistry.h"
#include "Entity.h"
#include "Components.h"
#include "systems/PlayerController.h"
#include "systems/ThrottleSystem.h"
#include "systems/RenderSystem.h"
#include "systems/MoveTranslateSystem.h"
#include <iostream>

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
	m_Renderer = SDL_CreateRenderer(window,-1,0);
	m_EventHandle = std::make_shared<SDL_Event>();


	m_ECSRegistry = std::make_shared<ECSRegistry>();
	m_ECSRegistry->Init();
	//TODO register components for later use

	m_ECSRegistry->RegisterComponent<TransformComp>();
	m_ECSRegistry->RegisterComponent<InputComp>();
	m_ECSRegistry->RegisterComponent<TagComp>();
	m_ECSRegistry->RegisterComponent<RigidBodyComp>();



	playerEntity = &CreateEntity("Test");
	auto& inputComp = playerEntity->AddComponent<InputComp>();
	auto& comp = playerEntity->AddComponent<TransformComp>();
	auto& tagComp = playerEntity->AddComponent<TagComp>();
	playerEntity->AddComponent<RigidBodyComp>();

	tagComp.Tag = ObjectTag::Player;

	comp.Position = Vector2(640.f, 400.f);
	comp.Size = Vector2(15.f, 15.f);


	for (size_t i = 0; i < 10; i++)
	{
		std::string name = "Enemy";
		name += std::to_string(i);
		auto testEnemy = &CreateEntity(name);
		auto& comp2 = testEnemy->AddComponent<TransformComp>();
		auto& tagComp2 = testEnemy->AddComponent<TagComp>();
		testEnemy->AddComponent<RigidBodyComp>();

		tagComp2.Tag = ObjectTag::Enemy;

		comp2.Position = Vector2(100.f * (i+1), 400.f);
		comp2.Size = Vector2(20.f, 20.f);

		
	}

	m_PlayerController = std::make_unique<PlayerController>(m_EventHandle, inputComp);
	m_ThrottleSystem = std::make_unique<ThrottleSystem>();
	m_RenderSystem = std::make_unique<RenderSystem>();
	m_MoveTranslateSystem = std::make_unique<MoveTranslateSystem>(m_Width,m_Height);
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
		
		//TODO make systems use observer pattern to only get new entites and components when they get added,
		//otherwise store reference to components at the creation of systems

		m_PlayerController->Update();

		m_ThrottleSystem->Update(m_ECSRegistry, deltaTime);

		m_RenderSystem->Update(m_ECSRegistry, m_Renderer, deltaTime);

		m_MoveTranslateSystem->Update(m_ECSRegistry,m_Renderer, deltaTime);

		SDL_RenderPresent(m_Renderer);
	}
}

std::shared_ptr<ECSRegistry> Game::GetECSRegistry()
{
	return m_ECSRegistry;
}

Entity& Game::CreateEntity(const std::string& name)
{
	return m_ECSRegistry->CreateEntity(name,this);
}