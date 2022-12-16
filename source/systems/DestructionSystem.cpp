#include "DestructionSystem.h"
#include "../ECS/ECSCore.h"
#include "../ECS/ECSRegistry.h"
#include "../ECS/Components.h"
#include "../containers/FixedSizeSet.h"

void DestructionSystem::Update(float deltaTime)
{
	auto& aliveEntities = m_Registry->GetActiveEntities();

	for (auto& entityId : aliveEntities)
	{
		if (entityId == nullptr) { continue; }

		auto& healthComp = m_Registry->GetComponent<HealthComp>(*entityId);
	
		if (healthComp.IsQueuedForDestroy)
		{
			auto& tagComp = m_Registry->GetComponent<TagComp>(*entityId);
			if(tagComp.Tag == ObjectTag::Player)
			{
				m_IsGameRunning = false;
			}
			auto& transformComp = m_Registry->GetComponent<TransformComp>(*entityId);
			m_QuadTree->Remove(*entityId, transformComp.Position.x, transformComp.Position.y);
			m_Registry->DestroyEntity(*entityId);
		}
	}
}
