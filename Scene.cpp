#include "ECSRegistry.h"
#include "Scene.h"
#include "Entity.h"
#include "SDL.h"
#include "Components.h"

Scene::Scene(struct SDL_Renderer* renderer) : m_Renderer(renderer)
{
	m_ECSRegistry = std::make_shared<ECSRegistry>();
	m_ECSRegistry->Init();
	testEntity = &CreateEntity("test");
	//TODO register components for later use

	m_ECSRegistry->RegisterComponent<TransformComp>();

	auto& comp=testEntity->AddComponent<TransformComp>();
	comp.Position = Vector2(640.f, 400.f);
	comp.Size = Vector2(10.f, 10.f);
}

Scene::~Scene()
{
}

void Scene::OnUpdate(float deltaTime)
{

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
	
	//SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);

	SDL_RenderPresent(m_Renderer);

}

Entity& Scene::CreateEntity(const std::string& name)
{
	return m_ECSRegistry->CreateEntity(name,this);
}

std::shared_ptr<ECSRegistry> Scene::GetECSRegistry()
{
	return m_ECSRegistry; 
}
