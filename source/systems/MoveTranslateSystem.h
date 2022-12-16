#pragma once
#include "ISystem.h"
#include "../ECS/SystemView.h"
#include "../ECS/Components.h"
#include "../containers/QuadTree.h"
#include <memory>

class MoveTranslateSystem : public ISystem
{
public:

	MoveTranslateSystem(std::shared_ptr<QuadTree<EntityID>> quadTree,std::shared_ptr<SystemView<TransformComp,RigidBodyComp, ColliderComp>> componentView) :m_QuadTree(quadTree), m_SystemView(componentView) {}
	~MoveTranslateSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	
	std::shared_ptr<QuadTree<EntityID>> m_QuadTree;
	std::shared_ptr<SystemView<TransformComp,RigidBodyComp,ColliderComp>> m_SystemView;
};
