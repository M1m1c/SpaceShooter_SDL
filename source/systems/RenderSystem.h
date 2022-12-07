#pragma once
#include <memory>
class ECSRegistry;
struct SDL_Renderer;
struct TransformComp;
struct SDL_Rect;

class RenderSystem
{
public:
	RenderSystem()=default;
	~RenderSystem()= default;

	void Update(const std::shared_ptr<ECSRegistry>& registry, SDL_Renderer* renderer, float deltaTime);


private:
	void RenderPlayer(TransformComp& transform, SDL_Renderer* renderer);

	void RenderEnemy(TransformComp& transform, SDL_Renderer* renderer);

	void RenderBullet(TransformComp& transform, SDL_Renderer* renderer);

	SDL_Rect GenerateQuad(TransformComp& transform);

};