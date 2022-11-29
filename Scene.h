#pragma once
#include <string>
#include <memory>


class ECSRegistry;
class Entity;

class Scene
{
public:
	Scene();
	~Scene();

	void OnUpdate(float deltaTime);

	Entity CreateEntity(const std::string& name = std::string());

	std::shared_ptr<ECSRegistry> GetECSRegistry();

private:

	std::shared_ptr<ECSRegistry> m_ECSRegistry;
};