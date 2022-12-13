#include "EntitySpawnSystem.h"
#include "../ECSRegistry.h"
#include "../SpawnOrder.h"

void EntitySpawnSystem::Update(float deltaTime)
{
	while (!m_OrderQueue.empty())
	{
		auto& order = m_OrderQueue.front();

		if (order.Tag == ObjectTag::Bullet)
		{
			auto bullet = m_Registry->CreateEntity<RigidBodyComp, InputComp>(
				Vector4(order.SpawnPosition.x, order.SpawnPosition.y, 5.f, 5.f),
				ObjectTag::Bullet);

			auto& rigidBody = m_Registry->GetComponent<RigidBodyComp>(bullet);
			auto& input = m_Registry->GetComponent<InputComp>(bullet);

			rigidBody.acceleration = 500.f;

			input.InputSignature = order.InputSignature;
		}
		else if (order.Tag == ObjectTag::Enemy)
		{
			auto enemy = m_Registry->CreateEntity<InputComp, RigidBodyComp>(
				Vector4(order.SpawnPosition.x, order.SpawnPosition.y, 20.f, 20.f),
				ObjectTag::Enemy);

			auto& input = m_Registry->GetComponent<InputComp>(enemy);
			input.InputSignature = order.InputSignature;
		}

		m_OrderQueue.pop();
	}
}
