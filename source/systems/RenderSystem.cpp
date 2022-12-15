#include "RenderSystem.h"
#include "SDL.h"
#include "../ECSCore.h"
#include "../Components.h"

void RenderSystem::Update(float deltaTime)
{
	auto& table = m_SystemView->GetComponents();

	for (size_t i = 0; i < table.size(); i++)
	{
		auto transform = std::get<0>(table[i]);
		auto tag = std::get<1>(table[i]).Tag;

		switch (tag)
		{
		case ObjectTag::None:
			break;
		case ObjectTag::Player:
			RenderPlayer(transform);
			break;
		case ObjectTag::Enemy:
			RenderEnemy(transform);
			break;
		case ObjectTag::Bullet:
			RenderBullet(transform);
			break;
		default:
			break;
		}
	}
}

void RenderSystem::RenderPlayer(TransformComp& transform)
{

	SDL_Rect rect = GenerateQuad(transform);
	

	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(m_Renderer, &rect);

	auto halfW = (rect.w * 0.5f);

	auto cX = rect.x;
	auto cY = rect.y + rect.h;
	SDL_RenderDrawLine(m_Renderer, cX, cY, cX - halfW, cY + rect.h);
	cX = rect.x + rect.w;
	SDL_RenderDrawLine(m_Renderer, cX, cY, cX + halfW, cY + rect.h);

	cX = rect.x + halfW;
	SDL_RenderDrawLine(m_Renderer, cX - 2.f, cY, cX - 4.f, cY + rect.h-2.f);
	SDL_RenderDrawLine(m_Renderer, cX + 2.f, cY, cX + 4.f, cY + rect.h-2.f);
}


void RenderSystem::RenderEnemy(TransformComp& transform)
{
	SDL_Rect rect = GenerateQuad(transform);

	SDL_SetRenderDrawColor(m_Renderer, 240, 0, 0, 255);
	SDL_RenderFillRect(m_Renderer, &rect);
}

void RenderSystem::RenderBullet(TransformComp& transform)
{
	SDL_Rect rect = GenerateQuad(transform);

	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(m_Renderer, &rect);
}


SDL_Rect RenderSystem::GenerateQuad(TransformComp& transform)
{
	auto min = transform.Position - transform.Size;
	auto max = transform.Position + transform.Size;
	auto width = max.x - min.x;
	auto height = max.y - min.y;

	SDL_Rect rect;
	rect.x = transform.Position.x - (width * 0.25f);
	rect.y = transform.Position.y - (height * 0.25f);
	rect.w = transform.Size.x;
	rect.h = transform.Size.y;
	return rect;
}
