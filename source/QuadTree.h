#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <memory>
#include <functional>

struct AABB
{
	float min_x, min_y;
	float max_x, max_y;

	AABB(float min_x, float min_y, float max_x, float max_y)
		: min_x(min_x), min_y(min_y), max_x(max_x), max_y(max_y)
	{
	}

	bool IsIntersecting(const AABB& other) const
	{
		return min_x <= other.max_x && max_x >= other.min_x &&
			min_y <= other.max_y && max_y >= other.min_y;
	}
};

template<typename T>
class QuadTreeNode
{
public:
	QuadTreeNode(float min_x, float min_y, float max_x, float max_y)
		: m_AABB(min_x, min_y, max_x, max_y)
	{
	}

	const AABB& GetAABB() const { return m_AABB; }

	bool IsLeaf() const { return m_Children[0] == nullptr; }

	std::shared_ptr<QuadTreeNode> GetChild(int index) const
	{
		assert(index >= 0 && index < 4);
		return m_Children[index];
	}


	void Traverse(std::function<void(QuadTreeNode*)> func)
	{
		func(this);

		for (int i = 0; i < 4; i++)
		{
			if (m_Children[i] != nullptr)
			{
				m_Children[i]->Traverse(func);
			}
		}
	}


	void GenerateChildren(float threshold)
	{
		auto belowSizeThreshold = m_AABB.max_x - m_AABB.min_x <= threshold || m_AABB.max_y - m_AABB.min_y <= threshold;
		if (belowSizeThreshold) { return; }


		float cx = (m_AABB.min_x + m_AABB.max_x) / 2;
		float cy = (m_AABB.min_y + m_AABB.max_y) / 2;

		m_Children[0] = std::make_shared<QuadTreeNode>(m_AABB.min_x, m_AABB.min_y, cx, cy);
		m_Children[1] = std::make_shared<QuadTreeNode>(cx, m_AABB.min_y, m_AABB.max_x, cy);
		m_Children[2] = std::make_shared<QuadTreeNode>(m_AABB.min_x, cy, cx, m_AABB.max_y);
		m_Children[3] = std::make_shared<QuadTreeNode>(cx, cy, m_AABB.max_x, m_AABB.max_y);

		for (int i = 0; i < 4; i++)
		{
			m_Children[i]->GenerateChildren(threshold);
		}
	}

private:

	AABB m_AABB;
	std::shared_ptr<QuadTreeNode> m_Children[4] = { nullptr, nullptr, nullptr, nullptr };
	std::vector<T> m_Data{};
};

template<typename T>
class QuadTree
{
public:
	// Construct a new quadtree with the given bounds, stopping division when threshold is reached
	QuadTree(float min_x, float min_y, float max_x, float max_y, float cellSizeThreshold = 50.f)
		: m_Root(std::make_shared<QuadTreeNode<T>>(min_x, min_y, max_x, max_y))
	{
		m_Root->GenerateChildren(cellSizeThreshold);
	}

	void Traverse(std::function<void(QuadTreeNode<T>*)> func)
	{
		m_Root->Traverse(func);
	}

private:
	std::shared_ptr<QuadTreeNode<T>> m_Root;
};
