#pragma once
#include "ISystem.h"
#include "../ECS/SystemView.h"
#include "../ECS/Components.h"
#include "../containers/QuadTree.h"
#include <memory>
#include <glm/glm.hpp>

//struct SDL_Renderer;

class CollisionSystem : public ISystem
{
public:
	CollisionSystem(std::shared_ptr<QuadTree<EntityID>> quadTree ,std::shared_ptr<SystemView<TransformComp, TagComp, HealthComp >> systemView) : m_QuadTree(quadTree), m_SystemView(systemView) {}
	~CollisionSystem() = default;

	virtual void Update(float deltaTime) override;

private:

	std::shared_ptr<QuadTree<EntityID>> m_QuadTree;
	std::shared_ptr<SystemView<TransformComp, TagComp, HealthComp >> m_SystemView;

};

