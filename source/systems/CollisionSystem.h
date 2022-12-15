#pragma once
#include "ISystem.h"
#include "../SystemView.h"
#include "../Components.h"
#include <memory>
#include <glm/glm.hpp>

struct SDL_Renderer;

class CollisionSystem : public ISystem
{
public:
	CollisionSystem(SDL_Renderer* renderer ,std::shared_ptr<SystemView<TransformComp, TagComp, HealthComp >> systemView) : m_Renderer(renderer), m_SystemView(systemView) {}
	~CollisionSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	glm::vec2 getMin(TransformComp& transform) const { return transform.Position - (transform.Size * 0.5f); }
	glm::vec2 getMin(glm::vec2 position, glm::vec2 size) const { return position - (size * 0.5f); }
	glm::vec2 getMax(TransformComp& transform) const { return transform.Position + (transform.Size * 0.5f); }
	glm::vec2 getMax(glm::vec2 position, glm::vec2 size) const { return position + (size * 0.5f); }

	void DrawCollider(TransformComp& transformA, glm::vec2& collderSize, bool canMove, SDL_Renderer* renderer);
private:

	SDL_Renderer* m_Renderer;
	std::shared_ptr<SystemView<TransformComp, TagComp, HealthComp >> m_SystemView;

};

