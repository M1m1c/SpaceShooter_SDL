#include "RenderSystem.h"
#include "SDL.h"
#include "ECSCore.h"
#include "ECSRegistry.h"
#include "Entity.h"
#include "Components.h"

void RenderSystem::Update(const std::shared_ptr<ECSRegistry>& registry, SDL_Renderer* renderer, float deltaTime)
{

	SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
	SDL_RenderClear(renderer);

	DataTable<MAX_ENTITIES, TransformComp,TagComp> table;

	auto count = registry->GetAllComponentPairs<MAX_ENTITIES, TransformComp,TagComp>(table);

	for (size_t i = 0; i < count; i++)
	{
		auto transform = std::get<0>(table[i]);
		auto tag = std::get<1>(table[i]).Tag;

		switch (tag)
		{
		case None:
			break;
		case Player:
			RenderPlayer(transform, renderer);
			break;
		case Enemy:
			RenderEnemy(transform, renderer);
			break;
		case Bullet:
			break;
		default:
			break;
		}
	}


	SDL_RenderPresent(renderer);

}

void RenderSystem::RenderPlayer(TransformComp& transform, SDL_Renderer* renderer)
{
	SDL_Rect rect;
	rect.x = transform.Position.x;
	rect.y = transform.Position.y;
	rect.w = transform.Size.x;
	rect.h = transform.Size.y;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);

	auto cX = rect.x;
	auto cY = rect.y + 10.f;
	SDL_RenderDrawLine(renderer, cX, cY, cX - 5.f, cY + 10.f);
	cX = rect.x + 10.f;
	SDL_RenderDrawLine(renderer, cX, cY, cX + 5.f, cY + 10.f);
	cX = rect.x + 5.f;
	SDL_RenderDrawLine(renderer, cX - 2.f, cY, cX - 4.f, cY + 8.f);
	SDL_RenderDrawLine(renderer, cX + 2.f, cY, cX + 4.f, cY + 8.f);
}

void RenderSystem::RenderEnemy(TransformComp& transform, SDL_Renderer* renderer)
{
	SDL_Rect rect;
	rect.x = transform.Position.x;
	rect.y = transform.Position.y;
	rect.w = transform.Size.x;
	rect.h = transform.Size.y;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);

	/*auto cX = rect.x;
	auto cY = rect.y + 10.f;
	SDL_RenderDrawLine(renderer, cX, cY, cX - 5.f, cY + 10.f);
	cX = rect.x + 10.f;
	SDL_RenderDrawLine(renderer, cX, cY, cX + 5.f, cY + 10.f);
	cX = rect.x + 5.f;
	SDL_RenderDrawLine(renderer, cX - 2.f, cY, cX - 4.f, cY + 8.f);
	SDL_RenderDrawLine(renderer, cX + 2.f, cY, cX + 4.f, cY + 8.f);*/
}
