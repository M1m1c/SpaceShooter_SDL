#pragma once
#include "ISystem.h"
#include "../ECSCore.h"
#include "../SystemView.h"
#include "../FixedQueue.h"
#include <memory>
#include <glm/glm.hpp>
struct TransformComp;
struct InputComp;
struct TagComp;
struct WeaponComp;
struct SpawnOrder;

class WeaponSystem : public ISystem
{
public:

	WeaponSystem(
		FixedQueue<SpawnOrder, MAX_ENTITIES>& orderQueue,
		std::shared_ptr<SystemView<TransformComp, InputComp, TagComp, WeaponComp>> componentView)
		: m_OrderQueue(orderQueue), m_SystemView(componentView) {}

	~WeaponSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	void SpawnBullet(glm::vec2 position, int direction);
	FixedQueue<SpawnOrder, MAX_ENTITIES>& m_OrderQueue;
	std::shared_ptr<SystemView<TransformComp, InputComp, TagComp, WeaponComp>> m_SystemView;
};

