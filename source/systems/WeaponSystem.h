#pragma once
#include <memory>
#include <glm/glm.hpp>
class ECSRegistry;
class Game;

class WeaponSystem
{
public:
	WeaponSystem() = default;
	~WeaponSystem() = default;

	void Update(const std::shared_ptr<ECSRegistry>& registry, Game* game, float deltaTime);

private:
	void SpawnBullet(const std::shared_ptr<ECSRegistry>& registry, glm::vec2 position, int direction);

};

