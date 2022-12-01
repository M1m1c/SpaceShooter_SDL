#pragma once
#include <memory>
#include "ECSCore.h"
class Entity;
class ECSRegistry;

class MovementSystem
{
public:
	MovementSystem(std::shared_ptr<ECSRegistry> registry);
	~MovementSystem() = default;

	void Update(const EntityID& entityID, float deltaTime);
private:
	std::shared_ptr<ECSRegistry> m_ECSRegistry;
};