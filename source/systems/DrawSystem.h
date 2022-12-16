#pragma once
#include "ISystem.h"
#include "../ECS/SystemView.h"
#include <memory>
struct SDL_Renderer;
struct TransformComp;
struct TagComp;
struct SDL_Rect;

class DrawSystem : public ISystem
{
public:
	DrawSystem(SDL_Renderer* renderer, std::shared_ptr<SystemView<TransformComp, TagComp>> componentView) :m_Renderer(renderer), m_SystemView(componentView) {}
	~DrawSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	void RenderPlayer(TransformComp& transform);

	void RenderEnemy(TransformComp& transform);

	void RenderBullet(TransformComp& transform);

	SDL_Rect GenerateQuad(TransformComp& transform);

	SDL_Renderer* m_Renderer;
	std::shared_ptr<SystemView<TransformComp, TagComp>> m_SystemView;
};