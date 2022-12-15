#include "DestructionSystem.h"
#include "../ECSCore.h"
#include "../ECSRegistry.h"
#include "../Components.h"
#include "../FixedSizeSet.h"

void DestructionSystem::Update(float deltaTime)
{
	auto& aliveEntities = m_Registry->GetActiveEntities();

	for (auto& entityId : aliveEntities)
	{
		if (entityId == nullptr) { continue; }
		auto& healthComp = m_Registry->GetComponent<HealthComp>(*entityId);

		auto isQueuedForDestroy = healthComp.IsQueuedForDestroy;
		auto isDead = healthComp.Health <= 0;

		if (isQueuedForDestroy || isDead)
		{
			auto tagComp = m_Registry->GetComponent<TagComp>(*entityId);
			if(tagComp.Tag == ObjectTag::Player)
			{
				m_IsGameRunning = false;
			}

			m_Registry->DestroyEntity(*entityId);
		}
	}
}
