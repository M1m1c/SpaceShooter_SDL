#pragma once
#include "ISystem.h"
#include "../SystemView.h"
#include "../Components.h"
#include "../QuadTree.h"
#include <memory>

class MoveTranslateSystem : public ISystem
{
public:

	MoveTranslateSystem(std::shared_ptr<QuadTree<EntityID>> quadTree,std::shared_ptr<SystemView<TransformComp,RigidBodyComp>> componentView) :m_QuadTree(quadTree), m_SystemView(componentView) {}
	~MoveTranslateSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	
	std::shared_ptr<QuadTree<EntityID>> m_QuadTree;
	std::shared_ptr<SystemView<TransformComp,RigidBodyComp>> m_SystemView;
};
