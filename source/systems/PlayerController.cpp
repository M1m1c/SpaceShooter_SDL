#include "PlayerController.h"

PlayerController::PlayerController(std::shared_ptr<SDL_Event> eventHandle, InputComp& playerInputComp) :
	m_EventHandle(eventHandle), m_PlayerInputComp(playerInputComp)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Update()
{
	SDL_PollEvent(m_EventHandle.get());
	auto type = m_EventHandle->type;
	auto press = type == SDL_KEYDOWN;
	auto Release = type == SDL_KEYUP;

	if (press || Release)
	{
		uint8_t value = press ? 1 : 0;
		switch (m_EventHandle->key.keysym.sym)
		{
		default:
			break;
		case SDLK_w:
		case SDLK_UP:
			m_PlayerInputComp.InputSignature[(int)Inputs::Up] = value;
			break;

		case SDLK_s:
		case SDLK_DOWN:
			m_PlayerInputComp.InputSignature[(int)Inputs::Down] = value;
			break;

		case SDLK_a:
		case SDLK_LEFT:
			m_PlayerInputComp.InputSignature[(int)Inputs::Left] = value;
			break;

		case SDLK_d:
		case SDLK_RIGHT:
			m_PlayerInputComp.InputSignature[(int)Inputs::Right] = value;
			break;

		case SDLK_SPACE:
			m_PlayerInputComp.InputSignature[(int)Inputs::Shoot] = value;
			break;
		}
	}
}
