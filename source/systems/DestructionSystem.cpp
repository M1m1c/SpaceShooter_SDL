#include "DestructionSystem.h"
#include "../ECSCore.h"
#include "../ECSRegistry.h"
#include "../Components.h"

void DestructionSystem::Update(float deltaTime)
{
	auto aliveEntities = m_Registry->GetActiveEntities();
	for (auto entityId : aliveEntities)
	{
		auto healthComp = m_Registry->GetComponent<HealthComp>(entityId);

		auto isQueuedForDestroy = healthComp.IsQueuedForDestroy;
		auto isDead = healthComp.Health <= 0;

		if (isQueuedForDestroy || isDead)
		{
			auto tagComp = m_Registry->GetComponent<TagComp>(entityId);
			if(tagComp.Tag == ObjectTag::Player)
			{
				//TODO close application
			}

			m_Registry->DestroyEntity(entityId);
		}
	}
}