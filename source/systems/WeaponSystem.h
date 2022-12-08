#pragma once
#include "ISystem.h"
#include <memory>
#include <glm/glm.hpp>
class ECSRegistry;
class Game;

class WeaponSystem : public ISystem
{
public:
	WeaponSystem(const std::shared_ptr<ECSRegistry>& registry): m_Registry(registry){}
	~WeaponSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	void SpawnBullet( glm::vec2 position, int direction);
	const std::shared_ptr<ECSRegistry>& m_Registry;
};

