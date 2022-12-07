#pragma once
#include <memory>
#include <glm/glm.hpp>
class ECSRegistry;

class WeaponSystem
{
public:
	WeaponSystem() = default;
	~WeaponSystem() = default;

	void Update(const std::shared_ptr<ECSRegistry>& registry, Game* game, float deltaTime);

	void SpawnBullet(Game* game, glm::vec2 position, int direction);

};

