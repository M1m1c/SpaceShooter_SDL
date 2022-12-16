#pragma once
#include "ISystem.h"
#include "../ECS/ECSCore.h"
#include "../containers/FixedQueue.h"
class ECSRegistry;
struct SpawnOrder;

class EntitySpawnSystem :public ISystem
{
public:
	EntitySpawnSystem(
		const std::shared_ptr<ECSRegistry>& registry, 
		FixedQueue<SpawnOrder, MAX_ENTITIES>& orderQueue) :
		m_Registry(registry), 
		m_OrderQueue(orderQueue) {}

	~EntitySpawnSystem()=default;

	virtual void Update(float deltaTime) override;
private:

	const std::shared_ptr<ECSRegistry>& m_Registry;
	FixedQueue<SpawnOrder, MAX_ENTITIES>& m_OrderQueue;
};

