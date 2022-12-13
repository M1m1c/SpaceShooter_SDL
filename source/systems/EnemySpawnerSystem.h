#pragma once
#include "ISystem.h"
class ECSRegistry;

class EnemySpawnerSystem : public ISystem
{
public:
	EnemySpawnerSystem(const std::shared_ptr<ECSRegistry>& registry) : m_Registry(registry){}
	~EnemySpawnerSystem() = default;

	virtual void Update(float deltaTime) override;
private:

	const std::shared_ptr<ECSRegistry>& m_Registry;

	float m_WaveSpawnTime = 0.f;
	float m_MaxWaveSpawnTime = 4.f;
};

