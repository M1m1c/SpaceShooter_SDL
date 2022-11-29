#include "ECSRegistry.h"
#include "Scene.h"
#include "Entity.h"

Scene::Scene()
{
	m_ECSRegistry = std::make_shared<ECSRegistry>();
	m_ECSRegistry->Init();
}

Scene::~Scene()
{
}

void Scene::OnUpdate(float deltaTime)
{
}

Entity Scene::CreateEntity(const std::string& name)
{
	return m_ECSRegistry->CreateEntity(name,this);
}

std::shared_ptr<ECSRegistry> Scene::GetECSRegistry()
{
	return m_ECSRegistry; 
}
