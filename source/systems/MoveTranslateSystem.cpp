#include "MoveTranslateSystem.h"
#include "../ECS/ECSCore.h"

void MoveTranslateSystem::Update(float deltaTime)
{
	auto& table = m_SystemView->GetComponents();
	auto size = table.size();
	for (int i = 0; i < size; ++i)
	{
		auto& transform = std::get<0>(table[i]);
		auto& colliderComp = std::get<2>(table[i]);
		
		auto nextPos = transform.NextPosition;
		
		auto nextCol = AABB(transform.NextPosition, transform.Size);
		m_QuadTree->Update(m_SystemView->GetID(i), nextCol, colliderComp.Collider);

		transform.Position = transform.NextPosition;
		colliderComp.Collider = nextCol;
		
	}
}

