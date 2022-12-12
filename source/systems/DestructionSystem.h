#pragma once
#include "ISystem.h"
#include <memory>
class ECSRegistry;

class DestructionSystem : public ISystem
{
public:
	DestructionSystem(const std::shared_ptr<ECSRegistry>& registry) : m_Registry(registry) {}
	~DestructionSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	const std::shared_ptr<ECSRegistry>& m_Registry;

};

