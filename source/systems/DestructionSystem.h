#pragma once
#include "ISystem.h"
#include "../ECS/ECSCore.h"
#include "../containers/QuadTree.h"
#include <memory>
class ECSRegistry;

class DestructionSystem : public ISystem
{
public:
	DestructionSystem(bool& isGameRunning, std::shared_ptr<QuadTree<EntityID>> quadTree, const std::shared_ptr<ECSRegistry>& registry) : m_IsGameRunning(isGameRunning), m_QuadTree(quadTree), m_Registry(registry) {}
	~DestructionSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	bool& m_IsGameRunning;
	std::shared_ptr<QuadTree<EntityID>> m_QuadTree;
	const std::shared_ptr<ECSRegistry>& m_Registry;

};

