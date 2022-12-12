#pragma once
#include "ISystem.h"
#include "../ComponentView.h"
#include <memory>
struct SDL_Renderer;
struct TransformComp;
struct TagComp;
struct SDL_Rect;

class RenderSystem : public ISystem
{
public:
	RenderSystem(SDL_Renderer* renderer, std::shared_ptr<ComponentView<TransformComp, TagComp>> componentView) :m_Renderer(renderer), m_ComponentView(componentView) {}
	~RenderSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	void RenderPlayer(TransformComp& transform);

	void RenderEnemy(TransformComp& transform);

	void RenderBullet(TransformComp& transform);

	SDL_Rect GenerateQuad(TransformComp& transform);

	SDL_Renderer* m_Renderer;
	std::shared_ptr<ComponentView<TransformComp, TagComp>> m_ComponentView;
};