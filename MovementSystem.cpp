#include "MovementSystem.h"
#include "ECSCore.h"
#include "ECSRegistry.h"
#include "Entity.h"
#include "Components.h"


//TODO instead of moving position imedietly have input affect rigidbody velocity,
// then later we should update position based on rigidbody velocity
//TODO have the systems store refernces to component pairs it cares about at creation,
// update the references array using observer pattern looking at updates to teh arrays it cares about,
// and checking if the entity whos components was removed is one the system cares about.
// only then should we make sure to update the references tehy system store.
void MovementSystem::Update(const std::shared_ptr<ECSRegistry>& registry, float deltaTime)
{
	/*DataTable<MAX_ENTITIES, TransformComp, InputComp> table;
	auto pairCount = registry->GetAllComponentPairs<MAX_ENTITIES, TransformComp, InputComp>(table);*/

	auto entityIDs = registry->GetEntityIDsMatchingSignature<TransformComp, InputComp>();

	for (size_t i = 0; i < entityIDs.size(); i++)
	{
		//auto& transformComp = std::get<0>(table[i]);//m_ECSRegistry->GetComponent<TransformComp>(entityID);
		//auto& input = std::get<1>(table[i]).InputSignature;//m_ECSRegistry->GetComponent<InputComp>(entityID).InputSignature;
		auto& transformComp = registry->GetComponent<TransformComp>(entityIDs[i]);
		auto input = registry->GetComponent<InputComp>(entityIDs[i]).InputSignature;

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
