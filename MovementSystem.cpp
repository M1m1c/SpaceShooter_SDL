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
	inputDir.Y = (input[Inputs::Down] == 1 ? 1.f : (input[Inputs::Up] == 1 ? -1.f : 0.f));

	//TODO maybe just decide to use glm for vectors and math
	//TODO normalise inputDir
	/*auto combined = inputDir.X + inputDir.Y;
	inputDir.X = combined / 2.f;
	inputDir.Y = combined / 2.f;*/
	//inputDir.Normalize();

	Vector2 moveStep = inputDir * 250.f * deltaTime;

	transformComp.Position += moveStep;
}
