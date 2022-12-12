#pragma once
#include "ISystem.h"
#include "../ComponentView.h"
#include <memory>
struct RigidBodyComp;
struct InputComp;

class ThrottleSystem: public ISystem
{
public:
	ThrottleSystem(std::shared_ptr<ComponentView<RigidBodyComp,InputComp>> componentView) : m_ComponentView(componentView){}
	~ThrottleSystem() = default;

	virtual void Update(float deltaTime) override;
private:
	std::shared_ptr<ComponentView<RigidBodyComp, InputComp>> m_ComponentView;
};