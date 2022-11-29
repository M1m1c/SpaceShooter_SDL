#pragma once
#include <string>

class Entity;
class ECSRegistry;

class Scene
{
public:
	Scene();
	~Scene();

	void OnUpdate(float deltaTime);

	Entity CreateEntity(const std::string& name = std::string());

	ECSRegistry& GetECSRegistry();

private:

	ECSRegistry m_ECSRegistry;
};