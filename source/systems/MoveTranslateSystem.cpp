#include "MoveTranslateSystem.h"
#include "../ECSCore.h"
#include "SDL.h"
#include <iostream>


//TODO this system does too much, break it up into smaller parts
void MoveTranslateSystem::Update(float deltaTime)
{

	auto table = m_ComponentView->GetComponents();
	auto size = table.size();
	for (int i = 0; i < size; ++i)
	{
		bool canMove = true;
		auto& transformA = std::get<0>(table[i]);
		auto& rigidBody = std::get<1>(table[i]);
		auto& tagA = std::get<2>(table[i]);
		auto& healthCompA = std::get<3>(table[i]);

		auto nextPosition = transformA.Position + rigidBody.velocity;

		//TODO should only stop player from moving, should queue destroy on other things
		//Boundry check
		if (nextPosition.x <= 0.f ||
			nextPosition.x >= m_ScreenWidth ||
			nextPosition.y <= 0.f ||
			nextPosition.y >= m_ScreenHeight)
		{
			canMove = false;

			if (tagA.Tag == ObjectTag::Bullet) 
			{ 
				healthCompA.IsQueuedForDestroy = true;
			}
		}


		const glm::vec2 aMin = getMin(nextPosition, transformA.Size);
		const glm::vec2 aMax = getMax(nextPosition, transformA.Size);
		glm::vec2 collderSize = glm::vec2(aMax.x - aMin.x, aMax.y - aMin.y);

		

		for (int j = 0; j < size; ++j)
		{
			if (j == i) { continue; }

			auto& tagB = std::get<2>(table[j]);
			auto& transformB = std::get<0>(table[j]);
			auto& HealthCompB = std::get<3>(table[j]);

			const glm::vec2 bMin = getMin(transformB);
			const glm::vec2 bMax = getMax(transformB);


			// Check if the AABBs intersect
			if (aMax.x >= bMin.x && aMin.x <= bMax.x && aMax.y >= bMin.y && aMin.y <= bMax.y)
			{

				//TODO stop us from moving, alternativley take damage, depending on our tag compared to other tag
				//also skip our own index
				//TODO add all events to a queue, go through queue when we are done chekcing all collisions

				if (tagA.Tag == ObjectTag::Bullet)
				{
					healthCompA.IsQueuedForDestroy = true;
					HealthCompB.Health--;
				}

				if (tagB.Tag == ObjectTag::Bullet) 
				{ 
					
					continue;
				}
				canMove = false;
			}
		}

		if (canMove) { transformA.Position += rigidBody.velocity; }

#ifdef _DEBUG
		DrawCollider(transformA, collderSize, canMove, m_Renderer);
#endif
	}
}

void MoveTranslateSystem::DrawCollider(TransformComp& transformA, glm::vec2& collderSize, bool canMove, SDL_Renderer* renderer)
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
