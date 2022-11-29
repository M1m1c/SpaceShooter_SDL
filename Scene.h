#pragma once
#include <string>

class Entity;

class Scene
{
public:
	Scene();
	~Scene();

	void OnUpdate(float deltaTime);

	Entity CreateEntity(const std::string& name = std::string());

private:

	//registry m_Registry;

};