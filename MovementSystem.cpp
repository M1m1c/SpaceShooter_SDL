#include "MovementSystem.h"
#include "ECSRegistry.h"
#include "Entity.h"
#include "Components.h"


//TODO instead of moving position imedietly have input affect rigidbody velocity,
// then later we should update position based on rigidbody velocity
void MovementSystem::Update(const std::shared_ptr<ECSRegistry>& registry, float deltaTime)
{
	DataTable<MAX_ENTITIES, TransformComp, InputComp> table;
	auto pairCount = registry->GetAllComponentPairs<MAX_ENTITIES, TransformComp, InputComp>(table);

	for (size_t i = 0; i < pairCount; i++)
	{
		auto& transformComp = std::get<0>(table[i]);//m_ECSRegistry->GetComponent<TransformComp>(entityID);
		auto input = std::get<1>(table[i]).InputSignature;//m_ECSRegistry->GetComponent<InputComp>(entityID).InputSignature;

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
	
}
