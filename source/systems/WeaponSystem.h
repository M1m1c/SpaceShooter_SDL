#pragma once
#include "ISystem.h"
#include "../ECSCore.h"
#include "../ComponentView.h"
#include <memory>
#include <glm/glm.hpp>
#include <queue>
struct TransformComp;
struct InputComp;
struct TagComp;
struct WeaponComp;
struct SpawnOrder;

class WeaponSystem : public ISystem
{
public:

	WeaponSystem(
		std::queue<SpawnOrder>& orderQueue,
		std::shared_ptr<ComponentView<TransformComp, InputComp, TagComp, WeaponComp>> componentView)
		: m_OrderQueue(orderQueue), m_ComponentView(componentView) {}

	~WeaponSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	void SpawnBullet(glm::vec2 position, int direction);
	std::queue<SpawnOrder>& m_OrderQueue;
	std::shared_ptr<ComponentView<TransformComp, InputComp, TagComp, WeaponComp>> m_ComponentView;
};

