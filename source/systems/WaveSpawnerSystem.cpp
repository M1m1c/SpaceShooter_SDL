#include "WaveSpawnerSystem.h"

#include "SpawnOrder.h"


void WaveSpawnerSystem::Update(float deltaTime)
{
	m_WaveSpawnTime -= deltaTime;
	if (m_WaveSpawnTime <= 0.f) 
	{
		std::mt19937 gen(m_RD());

		std::uniform_real_distribution<> disFloat(m_MinWaveSpawnTime, m_MaxWaveSpawnTime);
		float randomSpawnTime = disFloat(gen);
		m_WaveSpawnTime = randomSpawnTime;

		std::uniform_int_distribution<> disPatternInt(0, m_SpawnPatterns.size() - 1);
		int randomInt = disPatternInt(gen);
		auto& pattern = m_SpawnPatterns[randomInt];


		std::uniform_int_distribution<> disOffsetInt(-3, 3);
		randomInt = disOffsetInt(gen);


		for (size_t i = 0; i < pattern.Count; i++)
		{
			m_OrderQueue.push(SpawnOrder(
				Vector2(pattern.StartPosition.x + (pattern.StartOffset *randomInt) + (pattern.Offset.x * i), pattern.StartPosition.y + (pattern.Offset.y * i)),
				ObjectTag::Enemy,
				pattern.InputSignature
				));
		}

	}
}
