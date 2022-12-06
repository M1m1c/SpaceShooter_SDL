#pragma once
#include <memory>
class ECSRegistry;

class MovementSystem
{
public:
	MovementSystem() = default;
	~MovementSystem() = default;

	void Update(const std::shared_ptr<ECSRegistry>& registry, float deltaTime);
private:
	//std::shared_ptr<ECSRegistry> m_ECSRegistry;
};