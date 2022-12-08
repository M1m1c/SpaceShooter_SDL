#pragma once
#include <memory>
class ISystem
{
public:
	template<typename T,typename... Args>
	static std::unique_ptr<ISystem> Create(Args&&... args);

	virtual void Update(float deltaTime) = 0;
};

template<typename T, typename ...Args>
inline std::unique_ptr<ISystem> ISystem::Create(Args && ...args)
{
	return std::make_unique<T>(args...);
}
