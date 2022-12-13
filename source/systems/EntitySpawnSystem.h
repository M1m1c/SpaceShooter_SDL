#pragma once
#include "ISystem.h"
#include <queue>
class ECSRegistry;
struct SpawnOrder;

class EntitySpawnSystem :public ISystem
{
public:
	EntitySpawnSystem(
		const std::shared_ptr<ECSRegistry>& registry, 
		std::queue<SpawnOrder>& orderQueue) :
		m_Registry(registry), 
		m_OrderQueue(orderQueue) {}

	~EntitySpawnSystem()=default;

	virtual void Update(float deltaTime) override;
private:

	const std::shared_ptr<ECSRegistry>& m_Registry;
	std::queue<SpawnOrder>& m_OrderQueue;
};

