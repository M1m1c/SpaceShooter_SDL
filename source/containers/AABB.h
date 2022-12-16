#pragma once
#include "../ECS/Vectors.h"

struct AABB
{
	float min_x, min_y;
	float max_x, max_y;

	AABB(float min_x, float min_y, float max_x, float max_y)
		: min_x(min_x), min_y(min_y), max_x(max_x), max_y(max_y)
	{
	}

	AABB(Vector2 position, Vector2 size) : 
		min_x(position.x - (size.x * 0.5f)),
		min_y(position.y - (size.y * 0.5f)),
		max_x(position.x + (size.x * 0.5f)),
		max_y(position.y + (size.y * 0.5f))
	{
	}

	bool IsIntersecting(const AABB& other) const
	{
		return min_x <= other.max_x && max_x >= other.min_x &&
			min_y <= other.max_y && max_y >= other.min_y;
	}
};