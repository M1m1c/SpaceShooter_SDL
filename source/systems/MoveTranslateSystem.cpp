#include "MoveTranslateSystem.h"
#include "../ECS/ECSCore.h"

void MoveTranslateSystem::Update(float deltaTime)
{
	auto& table = m_SystemView->GetComponents();
	auto size = table.size();
	for (int i = 0; i < size; ++i)
	{
		auto& transform = std::get<0>(table[i]);
		
		auto nextPos = transform.NextPosition;
		auto oldPos = transform.Position;

		m_QuadTree->Update(m_SystemView->GetID(i), nextPos.x, nextPos.y, oldPos.x, oldPos.y);

		transform.Position = transform.NextPosition;
		
	}
}

