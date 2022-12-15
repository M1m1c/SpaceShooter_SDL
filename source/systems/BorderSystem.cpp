#include "BorderSystem.h"

void BorderSystem::Update(float deltaTime)
{
	//TODO this should actually only chakc things that are in border sectors
	auto& table = m_SystemView->GetComponents();

	for (int i = 0; i < table.size(); ++i)
	{
		auto& transform = std::get<0>(table[i]);
		auto& tagComp = std::get<1>(table[i]);
		auto& healthComp = std::get<2>(table[i]);
		auto& nextPosition = transform.NextPosition;
		if (nextPosition.y >= m_ScreenHeight && tagComp.Tag == ObjectTag::Enemy)
		{
			healthComp.IsQueuedForDestroy = true;
		}

		if (nextPosition.x <= 0.f ||
			nextPosition.x >= m_ScreenWidth ||
			nextPosition.y <= 0.f ||
			nextPosition.y >= m_ScreenHeight)
		{



			if (tagComp.Tag == ObjectTag::Bullet)
			{
				healthComp.IsQueuedForDestroy = true;
			}
			else if (tagComp.Tag == ObjectTag::Player)
			{
				//canMove = false;
				transform.NextPosition = transform.Position;
			}
		}
	}
}
