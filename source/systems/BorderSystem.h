#pragma once

#include "ISystem.h"
#include "../ECS/SystemView.h"
#include "../ECS/Components.h"
#include <memory>

class BorderSystem :public ISystem
{
public:
	BorderSystem(
		int width,
		int height,
		std::shared_ptr<SystemView<TransformComp, TagComp, HealthComp >> view) 
		:m_ScreenWidth(width), m_ScreenHeight(height), m_SystemView(view) {  }

	~BorderSystem()=default;

	virtual void Update(float deltaTime) override;

private:
	std::shared_ptr<SystemView<TransformComp, TagComp, HealthComp >> m_SystemView;
	int m_ScreenWidth, m_ScreenHeight;
};
