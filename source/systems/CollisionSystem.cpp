#include "CollisionSystem.h"
#include "../ECSCore.h"
#include "SDL.h"

void CollisionSystem::Update(float deltaTime)
{
	//TODO this should use a quad tree to only check sectors that have more than one entity with different tags

	auto& table = m_SystemView->GetComponents();
	auto size = table.size();
	for (int i = 0; i < size; ++i)
	{
		auto& transformA = std::get<0>(table[i]);
		auto& tagCompA = std::get<1>(table[i]);
		auto& healthCompA = std::get<2>(table[i]);
		auto& nextPositionA = transformA.NextPosition;

		const glm::vec2 aMin = getMin(nextPositionA, transformA.Size);
		const glm::vec2 aMax = getMax(nextPositionA, transformA.Size);
		glm::vec2 collderSize = glm::vec2(aMax.x - aMin.x, aMax.y - aMin.y);

		for (int j = 0; j < size; ++j)
		{
			if (j == i) { continue; }

			auto& transformB = std::get<0>(table[j]);
			auto& tagB = std::get<1>(table[j]);
			auto& healthCompB = std::get<2>(table[j]);

			const glm::vec2 bMin = getMin(transformB);
			const glm::vec2 bMax = getMax(transformB);



			// Check if the AABBs intersect
			if (aMax.x >= bMin.x && aMin.x <= bMax.x && aMax.y >= bMin.y && aMin.y <= bMax.y)
			{

				if (tagCompA.Tag == ObjectTag::Bullet)
				{
					healthCompA.IsQueuedForDestroy = true;
					healthCompB.Health--;
				}
				else if (tagCompA.Tag == ObjectTag::Player)
				{
					healthCompA.Health--;
				}
			}

		}

#ifdef _DEBUG
		DrawCollider(transformA, collderSize, true, m_Renderer);
#endif
	}
}

void CollisionSystem::DrawCollider(TransformComp& transformA, glm::vec2& collderSize, bool canMove, SDL_Renderer* renderer)
{
	SDL_Rect rect;
	rect.x = transformA.Position.x - (collderSize.x * 0.5f);
	rect.y = transformA.Position.y - (collderSize.y * 0.5f);
	rect.w = collderSize.x;
	rect.h = collderSize.y;

	glm::ivec3 color = canMove ? glm::ivec3(0, 255, 0) : glm::ivec3(255, 0, 0);

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawRect(renderer, &rect);
}
