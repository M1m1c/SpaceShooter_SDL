#pragma once

#include "ISystem.h"
#include "../ECS/SystemView.h"
#include "../ECS/Components.h"
#include <memory>

class MoveCalcSystem : public ISystem
{
public:
	MoveCalcSystem(std::shared_ptr<SystemView<TransformComp, RigidBodyComp>> view) : m_SystemView(view){}
	~MoveCalcSystem()=default;
	virtual void Update(float deltaTime) override;
private:
	std::shared_ptr<SystemView<TransformComp, RigidBodyComp>> m_SystemView;
};
