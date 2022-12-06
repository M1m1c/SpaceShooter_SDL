#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "../Components.h"
class ECSRegistry;
struct SDL_Renderer;

class MoveTranslateSystem
{
public:
	MoveTranslateSystem() = default;
	MoveTranslateSystem(int width, int height) :m_ScreenWidth(width), m_ScreenHeight(height) {}
	~MoveTranslateSystem() = default;

	void Update(const std::shared_ptr<ECSRegistry>& registry, SDL_Renderer* renderer, float deltaTime);


private:
	glm::vec2 getMin(TransformComp& transform) const { return transform.Position - (transform.Size * 0.5f); }
	glm::vec2 getMin(glm::vec2 position, glm::vec2 size) const { return position - (size * 0.5f); }
	glm::vec2 getMax(TransformComp& transform) const { return transform.Position + (transform.Size * 0.5f); }
	glm::vec2 getMax(glm::vec2 position, glm::vec2 size) const { return position + (size * 0.5f); }

	void DrawCollider(TransformComp& transformA, glm::vec2& collderSize, bool canMove, SDL_Renderer* renderer);

	int m_ScreenWidth, m_ScreenHeight;
};
