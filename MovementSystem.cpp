#include "MovementSystem.h"
#include "ECSRegistry.h"
#include "Entity.h"
#include "Components.h"

MovementSystem::MovementSystem(std::shared_ptr<ECSRegistry> registry) :
	m_ECSRegistry(registry)
{
}

//TODO instead of moving position imedietly have input affect rigidbody velocity,
// then later we should update position based on rigidbody velocity
void MovementSystem::Update(const EntityID& entityID, float deltaTime)
{
	auto input = m_ECSRegistry->GetComponent<InputComp>(entityID).InputSignature;
	auto& transformComp = m_ECSRegistry->GetComponent<TransformComp>(entityID);

	Vector2 inputDir;
	inputDir.x = (input[Inputs::Left] == 1 ? -1.f : (input[Inputs::Right] == 1 ? 1.f : 0.f));
	inputDir.y = (input[Inputs::Down] == 1 ? 1.f : (input[Inputs::Up] == 1 ? -1.f : 0.f));

	if (inputDir.x != 0.f || inputDir.y != 0.f)
	{
		inputDir = glm::normalize(inputDir);
	}

	Vector2 moveStep = inputDir * 250.f * deltaTime;

	transformComp.Position += moveStep;
}
