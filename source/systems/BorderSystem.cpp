#include "BorderSystem.h"

void BorderSystem::Update(float deltaTime)
{
	auto& table = m_SystemView->GetComponents();

	for (int i = 0; i < table.size(); ++i)
	{
		auto& transform = std::get<0>(table[i]);
		auto& tagComp = std::get<1>(table[i]);
		auto& healthComp = std::get<2>(table[i]);
		auto& nextPosition = transform.NextPosition;

		auto isBelowScreenBorder = nextPosition.y >= m_ScreenHeight;
		if (isBelowScreenBorder && tagComp.Tag == ObjectTag::Enemy)
		{
			healthComp.IsQueuedForDestroy = true;
		}

		auto isOutsideScreenWidth = nextPosition.x <= 0.f || nextPosition.x >= m_ScreenWidth;
		auto isOutsideScreenHeight = nextPosition.y <= 0.f || nextPosition.y >= m_ScreenHeight;
		if ( isOutsideScreenWidth || isOutsideScreenHeight)
		{

			if (tagComp.Tag == ObjectTag::Bullet)
			{
				healthComp.IsQueuedForDestroy = true;
			}
			else if (tagComp.Tag == ObjectTag::Player)
			{
				transform.NextPosition = transform.Position;
			}
		}
	}
}
