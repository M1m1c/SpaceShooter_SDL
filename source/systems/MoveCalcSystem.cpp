#include "MoveCalcSystem.h"

void MoveCalcSystem::Update(float deltaTime)
{
	auto& table = m_SystemView->GetComponents();

	for (int i = 0; i < table.size(); ++i)
	{
		auto& transform = std::get<0>(table[i]);
		auto& rigidBody = std::get<1>(table[i]);
		transform.NextPosition = transform.Position + rigidBody.velocity;
	}
}
