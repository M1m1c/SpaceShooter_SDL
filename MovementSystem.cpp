#include "MovementSystem.h"
#include "ECSRegistry.h"
#include "Entity.h"
#include "Components.h"

MovementSystem::MovementSystem(std::shared_ptr<ECSRegistry> registry):
	m_ECSRegistry(registry)
{
}

void MovementSystem::Update(const EntityID& entityID, float deltaTime)
{
	auto input = m_ECSRegistry->GetComponent<InputComp>(entityID).InputSignature;
	auto& transformComp = m_ECSRegistry->GetComponent<TransformComp>(entityID);

	Vector2 inputDir;
	inputDir.X = (input[Inputs::Left] == 1 ? -1.f : (input[Inputs::Right] == 1 ? 1.f : 0.f));
	inputDir.Y = (input[Inputs::Down] == 1 ? -1.f : (input[Inputs::Up] == 1 ? 1.f : 0.f));

	//TODO normalise inputDir

	Vector2 moveStep = inputDir * 100.f * deltaTime;

	transformComp.Position += moveStep;
}
