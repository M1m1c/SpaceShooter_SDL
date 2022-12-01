#include "Game.h"

#include "SDL.h"
#include "ECSRegistry.h"
#include "Entity.h"
#include "Components.h"
#include "PlayerController.h"
#include "MovementSystem.h"
#include <iostream>


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



	testEntity = &CreateEntity("Test");
	auto& inputComp = testEntity->AddComponent<InputComp>();
	auto& comp = testEntity->AddComponent<TransformComp>();

	comp.Position = Vector2(640.f, 400.f);
	comp.Size = Vector2(10.f, 10.f);

	m_PlayerController = std::make_unique<PlayerController>(m_EventHandle, inputComp);
	m_MovementSystem = std::make_unique<MovementSystem>(m_ECSRegistry);
}

void Game::Run()
{
	while (m_IsRunning)
	{
		float currentTime = SDL_GetTicks64();
		float deltaTime = (currentTime - m_LastFrameTime) * 0.001f;
		m_LastFrameTime = currentTime;
		
		//TODO make systems able to get the components they need to affect by themselves

		m_PlayerController->Update();

		/*auto position =testEntity->GetComponent<TransformComp>().Position;
		
		std::cout << "X = " << std::to_string(position.X)<<" Y = "<< std::to_string(position.Y) << std::endl;*/

		m_MovementSystem->Update(testEntity->GetID(), deltaTime);


		//TODO implement render system

		SDL_SetRenderDrawColor(m_Renderer, 20, 20, 30, 255);
		SDL_RenderClear(m_Renderer);

		if (testEntity)
		{
			auto transform = testEntity->GetComponent<TransformComp>();
			SDL_Rect rect;
			rect.x = transform.Position.X;
			rect.y = transform.Position.Y;
			rect.w = transform.Size.X;
			rect.h = transform.Size.Y;

			SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(m_Renderer, &rect);

		}

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
