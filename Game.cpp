#include "Game.h"

#include "SDL.h"
#include "ECSRegistry.h"
#include "Entity.h"
#include "Components.h"
#include "PlayerController.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include <iostream>

const float MILLI_TO_SECONDS = 0.001f;

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(SDL_Window* window, SDL_Surface* surface)
{
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



	playerEntity = &CreateEntity("Test");
	auto& inputComp = playerEntity->AddComponent<InputComp>();
	auto& comp = playerEntity->AddComponent<TransformComp>();
	auto& tagComp = playerEntity->AddComponent<TagComp>();

	tagComp.Tag = ObjectTag::Player;

	comp.Position = Vector2(640.f, 400.f);
	comp.Size = Vector2(10.f, 10.f);


	for (size_t i = 0; i < 10; i++)
	{
		std::string name = "Enemy";
		name += std::to_string(i);
		auto testEnemy = &CreateEntity(name);
		auto& comp2 = testEnemy->AddComponent<TransformComp>();
		auto& tagComp2 = testEnemy->AddComponent<TagComp>();

		tagComp2.Tag = ObjectTag::Enemy;

		comp2.Position = Vector2(100.f * (i+1), 400.f);
		comp2.Size = Vector2(20.f, 20.f);

		
	}

	m_PlayerController = std::make_unique<PlayerController>(m_EventHandle, inputComp);
	m_MovementSystem = std::make_unique<MovementSystem>();
	m_RenderSystem = std::make_unique<RenderSystem>();
}

void Game::Run()
{
	while (m_IsRunning)
	{
		float currentTime = SDL_GetTicks64();
		float deltaTime = (currentTime - m_LastFrameTime) * MILLI_TO_SECONDS;
		m_LastFrameTime = currentTime;
		
		//TODO make systems able to get the components they need to affect by themselves

		m_PlayerController->Update();

		/*auto position =testEntity->GetComponent<TransformComp>().Position;
		
		std::cout << "X = " << std::to_string(position.X)<<" Y = "<< std::to_string(position.Y) << std::endl;*/

		m_MovementSystem->Update(m_ECSRegistry, deltaTime);

		//TODO implement render system

		m_RenderSystem->Update(m_ECSRegistry, m_Renderer, deltaTime);
		
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
