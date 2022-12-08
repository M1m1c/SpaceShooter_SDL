#pragma once
#include "ISystem.h"
#include <memory>
class ECSRegistry;
struct SDL_Renderer;
struct TransformComp;
struct SDL_Rect;

class RenderSystem : public ISystem
{
public:
	RenderSystem(const std::shared_ptr<ECSRegistry>& registry, SDL_Renderer* renderer);
	~RenderSystem()= default;

	virtual void Update(float deltaTime) override;


private:
	void RenderPlayer(TransformComp& transform);

	void RenderEnemy(TransformComp& transform);

	void RenderBullet(TransformComp& transform);

	SDL_Rect GenerateQuad(TransformComp& transform);

	const std::shared_ptr<ECSRegistry>& m_Registry;
	SDL_Renderer* m_Renderer;

};