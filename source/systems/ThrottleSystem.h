#pragma once
#include "ISystem.h"
#include "../ECS/SystemView.h"
#include <memory>
struct RigidBodyComp;
struct InputComp;

class ThrottleSystem: public ISystem
{
public:
	ThrottleSystem(std::shared_ptr<SystemView<RigidBodyComp,InputComp>> componentView) : m_SystemView(componentView){}
	~ThrottleSystem() = default;

	virtual void Update(float deltaTime) override;
private:
	std::shared_ptr<SystemView<RigidBodyComp, InputComp>> m_SystemView;
};