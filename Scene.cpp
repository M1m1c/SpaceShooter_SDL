#include "Scene.h"
#include "Entity.h"
#include "ECSRegistry.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::OnUpdate(float deltaTime)
{
}

Entity Scene::CreateEntity(const std::string& name)
{
	return m_ECSRegistry.CreateEntity(name,this);
}

ECSRegistry& Scene::GetECSRegistry()
{
	return m_ECSRegistry; 
}
