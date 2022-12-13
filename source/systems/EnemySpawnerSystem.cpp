#include "EnemySpawnerSystem.h"

#include "../ECSCore.h"
#include "../ECSRegistry.h"


void EnemySpawnerSystem::Update(float deltaTime)
{
	m_WaveSpawnTime -= deltaTime;
	if (m_WaveSpawnTime <= 0.f) 
	{
		std::mt19937 gen(m_RD());

		std::uniform_real_distribution<> disFloat(m_MinWaveSpawnTime, m_MaxWaveSpawnTime);
		float randomSpawnTime = disFloat(gen);
		m_WaveSpawnTime = randomSpawnTime;

		std::uniform_int_distribution<> disInt(0, m_SpawnPatterns.size() - 1);
		int randomInt = disInt(gen);
		auto& pattern = m_SpawnPatterns[randomInt];


		//TODO change this so we can set patterns of how they spawn and randamize between them,
		//essentailly setup spawn parameters
		//TODO instead of spawning here make a spawn system that takes spawn orders, 
		//that way we compose spawn orders here and push them to a queue later handled by the spawn system.
		for (size_t i = 0; i < 10; i++)
		{
			auto enemy = m_Registry->CreateEntity<InputComp, RigidBodyComp>(
				Vector4(
					pattern.StartPosition.x + (pattern.Offset.x * i),
					pattern.StartPosition.y + (pattern.Offset.y * i),
					20.f, 20.f),
				ObjectTag::Enemy);

			auto& input = m_Registry->GetComponent<InputComp>(enemy);
			input.InputSignature = pattern.InputSignature;
		}

	}
}
