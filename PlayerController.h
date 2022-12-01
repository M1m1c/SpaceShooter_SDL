#pragma once
#include <memory>
#include "SDL.h"
#include "Components.h"

class PlayerController
{
public:
	PlayerController(std::shared_ptr<SDL_Event> eventHandle, InputComp& playerInputComp);
	~PlayerController();

	void Update();

private:
	std::shared_ptr<SDL_Event> m_EventHandle;
	InputComp& m_PlayerInputComp;
};