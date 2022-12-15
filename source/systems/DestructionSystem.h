#pragma once
#include "ISystem.h"
#include <memory>
class ECSRegistry;

class DestructionSystem : public ISystem
{
public:
	DestructionSystem(bool& isGameRunning, const std::shared_ptr<ECSRegistry>& registry) : m_IsGameRunning(isGameRunning), m_Registry(registry) {}
	~DestructionSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	bool& m_IsGameRunning;
	const std::shared_ptr<ECSRegistry>& m_Registry;

};

