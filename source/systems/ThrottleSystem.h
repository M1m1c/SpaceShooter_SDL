#pragma once
#include "ISystem.h"
#include <memory>
class ECSRegistry;

class ThrottleSystem: public ISystem
{
public:
	ThrottleSystem(const std::shared_ptr<ECSRegistry>& registry);
	~ThrottleSystem() = default;

	virtual void Update(float deltaTime) override;
private:
	const std::shared_ptr<ECSRegistry>& m_Registry;
};