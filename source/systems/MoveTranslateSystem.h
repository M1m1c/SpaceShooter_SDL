#pragma once
#include "ISystem.h"
#include <memory>
#include <glm/glm.hpp>
#include "../Components.h"
class ECSRegistry;
struct SDL_Renderer;

class MoveTranslateSystem : public ISystem
{
public:

	MoveTranslateSystem(const std::shared_ptr<ECSRegistry>& registry, SDL_Renderer* renderer,int width, int height) :
		m_Registry(registry),m_Renderer(renderer), m_ScreenWidth(width), m_ScreenHeight(height) {}
	~MoveTranslateSystem() = default;

	virtual void Update(float deltaTime) override;


private:
	glm::vec2 getMin(TransformComp& transform) const { return transform.Position - (transform.Size * 0.5f); }
	glm::vec2 getMin(glm::vec2 position, glm::vec2 size) const { return position - (size * 0.5f); }
	glm::vec2 getMax(TransformComp& transform) const { return transform.Position + (transform.Size * 0.5f); }
	glm::vec2 getMax(glm::vec2 position, glm::vec2 size) const { return position + (size * 0.5f); }

	void DrawCollider(TransformComp& transformA, glm::vec2& collderSize, bool canMove, SDL_Renderer* renderer);

	const std::shared_ptr<ECSRegistry>& m_Registry;
	SDL_Renderer* m_Renderer;
	int m_ScreenWidth, m_ScreenHeight;
};
