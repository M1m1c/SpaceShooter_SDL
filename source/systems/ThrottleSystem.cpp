#include "ThrottleSystem.h"
#include "../ECSCore.h"
#include "../ECSRegistry.h"
#include "../Components.h"

void ThrottleSystem::Update(float deltaTime)
{
	auto table = m_ComponentView->GetComponents();

	for (size_t i = 0; i < table.size(); i++)
	{
		auto& rigidBody = std::get<0>(table[i]);
		auto inputSig = std::get<1>(table[i]).InputSignature;


		Vector2 inputDir;
		inputDir.x = (inputSig[(int)Inputs::Left] == 1 ? -1.f : (inputSig[(int)Inputs::Right] == 1 ? 1.f : 0.f));
		inputDir.y = (inputSig[(int)Inputs::Down] == 1 ? 1.f : (inputSig[(int)Inputs::Up] == 1 ? -1.f : 0.f));

		if (inputDir.x != 0.f || inputDir.y != 0.f)
		{
			inputDir = glm::normalize(inputDir);
		}
		Vector2 moveStep = inputDir * rigidBody.acceleration * deltaTime;

		rigidBody.velocity = moveStep;
	}
}