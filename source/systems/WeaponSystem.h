#pragma once
#include "ISystem.h"
#include "../ComponentView.h"
#include <memory>
#include <glm/glm.hpp>
class ECSRegistry;
class Game;
struct TransformComp;
struct InputComp;
struct TagComp;
struct WeaponComp;

class WeaponSystem : public ISystem
{
public:

	WeaponSystem(const std::shared_ptr<ECSRegistry>& registry,std::shared_ptr<ComponentView<TransformComp, InputComp, TagComp, WeaponComp>> componentView): m_Registry(registry), m_ComponentView(componentView) {}
	~WeaponSystem() = default;

	virtual void Update(float deltaTime) override;

private:
	void SpawnBullet( glm::vec2 position, int direction);
	const std::shared_ptr<ECSRegistry>& m_Registry;
	std::shared_ptr<ComponentView<TransformComp, InputComp, TagComp, WeaponComp>> m_ComponentView;
};

