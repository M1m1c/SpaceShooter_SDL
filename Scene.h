#pragma once
#include <string>
#include <memory>


class ECSRegistry;
class Entity;

class Scene
{
public:
	Scene(struct SDL_Renderer* renderer);
	~Scene();

	void OnUpdate(float deltaTime);

	Entity& CreateEntity(const std::string& name = std::string());

	std::shared_ptr<ECSRegistry> GetECSRegistry();

private:

	std::shared_ptr<ECSRegistry> m_ECSRegistry;
	struct SDL_Renderer* m_Renderer;
	Entity* testEntity;

	//TODO just an idea, have the scene hold all the systems in an array or vector.
	//have the scene send refernces to all components relevant to the system in a tuple in the systems update call
};