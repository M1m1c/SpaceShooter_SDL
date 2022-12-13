#include "EnemySpawnerSystem.h"

#include "../ECSCore.h"
#include "../ECSRegistry.h"
#include "../Components.h"

void EnemySpawnerSystem::Update(float deltaTime)
{
	m_WaveSpawnTime -= deltaTime;
	if (m_WaveSpawnTime <= 0.f) 
	{
		//TODO randomise between min and max spawn time
		m_WaveSpawnTime = m_MaxWaveSpawnTime;

		//TODO change this so we can set patterns of how they spawn and randamize between them,
		//essentailly setup spawn parameters
		//TODO instead of spawning here make a spawn system that takes spawn orders, 
		//that way we compose spawn orders here and push them to a queue later handled by the spawn system.
		for (size_t i = 0; i < 10; i++)
		{
			auto enemy = m_Registry->CreateEntity<InputComp, RigidBodyComp>(
				Vector4(120.f * (i + 1), 0.f, 20.f, 20.f),
				ObjectTag::Enemy);

			auto& input = m_Registry->GetComponent<InputComp>(enemy);
			input.InputSignature[(int)Inputs::Down] = 1;
		}

	}
}
