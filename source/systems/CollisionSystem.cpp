#include "CollisionSystem.h"
#include "../ECS/ECSCore.h"

void CollisionSystem::Update(float deltaTime)
{
	auto& table = m_SystemView->GetComponents();
	auto view = m_SystemView;
	m_QuadTree->Traverse([&table, &view](QuadTreeNode<EntityID>* node)
		{
			auto& data = node->GetData();
			auto size = data.size();
			if (size > 1)
			{
				for (size_t i = 0; i < size; i++)
				{
					auto tableIndex_A = view->GetIndex(data[i]);
					auto& transformA = std::get<0>(table[tableIndex_A]);
					auto& tagCompA = std::get<1>(table[tableIndex_A]);
					auto& healthCompA = std::get<2>(table[tableIndex_A]);
					auto& nextPositionA = transformA.NextPosition;

					for (size_t j = i + 1; j < size; j++)
					{
						auto tableIndex_B = view->GetIndex(data[j]);
						auto& transformB = std::get<0>(table[tableIndex_B]);
						auto& tagCompB = std::get<1>(table[tableIndex_B]);
						auto& healthCompB = std::get<2>(table[tableIndex_B]);

						if (tagCompA.Tag != tagCompB.Tag)
						{
							const glm::vec2 aMin = nextPositionA - (transformA.Size * 0.5f);
							const glm::vec2 aMax = nextPositionA + (transformA.Size * 0.5f);
							const glm::vec2 bMin = transformB.Position - (transformB.Size * 0.5f);
							const glm::vec2 bMax = transformB.Position + (transformB.Size * 0.5f);

							// Check if the AABBs intersect
							if (aMax.x >= bMin.x && aMin.x <= bMax.x && aMax.y >= bMin.y && aMin.y <= bMax.y)
							{
								healthCompA.IsQueuedForDestroy = true;
								healthCompB.IsQueuedForDestroy = true;
							}
						}


					}
				}
			}

		});
}