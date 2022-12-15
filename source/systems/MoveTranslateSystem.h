#pragma once
#include "ISystem.h"
#include "../SystemView.h"
#include "../Components.h"
#include <memory>

class MoveTranslateSystem : public ISystem
{
public:

	MoveTranslateSystem(std::shared_ptr<SystemView<TransformComp,RigidBodyComp>> componentView) :m_SystemView(componentView) {}
	~MoveTranslateSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	
	std::shared_ptr<SystemView<TransformComp,RigidBodyComp>> m_SystemView;
};
