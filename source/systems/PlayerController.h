#pragma once
#include "ISystem.h"
#include <memory>
#include "SDL.h"
#include "../ECS/Components.h"

class PlayerController : public ISystem
{
public:
	PlayerController(std::shared_ptr<SDL_Event> eventHandle, InputComp& playerInputComp) :m_EventHandle(eventHandle), m_PlayerInputComp(playerInputComp) {}
	~PlayerController() = default;

	virtual void Update(float deltaTime) override;

private:
	std::shared_ptr<SDL_Event> m_EventHandle;
	InputComp& m_PlayerInputComp;
};