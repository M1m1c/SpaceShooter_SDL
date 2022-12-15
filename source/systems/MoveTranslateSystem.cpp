#include "MoveTranslateSystem.h"
#include "../ECSCore.h"

void MoveTranslateSystem::Update(float deltaTime)
{
	auto& table = m_SystemView->GetComponents();
	auto size = table.size();
	for (int i = 0; i < size; ++i)
	{
		auto& transform = std::get<0>(table[i]);
		
		transform.Position = transform.NextPosition;
	}
}

