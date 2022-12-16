#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <memory>
#include <functional>
#include "ECSCore.h"

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
		m_Data.reserve(MAX_ENTITIES * 0.1f);
	}

	const AABB& GetAABB() const { return m_AABB; }
	const std::vector<T>& GetData() const { return m_Data; }

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

	// Return the index of the child that contains the given point, or -1 if no such child exists
	int FindIndex(float x, float y) const
	{
		float cx = (m_AABB.min_x + m_AABB.max_x) / 2;
		float cy = (m_AABB.min_y + m_AABB.max_y) / 2;

		int index = -1;
		if (x < cx && y < cy) { index = 0; }
		else if (x >= cx && y < cy) { index = 1; }
		else if (x < cx && y >= cy) { index = 2; }
		else if (x >= cx && y >= cy) { index = 3; }

		return index;
	}

	QuadTreeNode* FindNode(float x, float y)
	{
		if (!IsLeaf())
		{
			// This node is not a leaf, so add the object to the appropriate child
			int index = FindIndex(x, y);
			if (index != -1)
			{
				return m_Children[index]->FindNode(x, y);
			}
		}
		else
		{
			//TODO if we cant finr posiiton within this node then it is out of bounds and we should return null
			auto isWithinX = x >= m_AABB.min_x && x <= m_AABB.max_x;
			auto isWithinY = y >= m_AABB.min_y && y <= m_AABB.max_y;
			if (isWithinX && isWithinY)
			{
				return this;
			}
			else
			{
				return nullptr;
			}
		}
	}

	void Add(T item, float x, float y)
	{
		int index = FindIndex(x, y);
		if (!IsLeaf())
		{
			// This node is not a leaf, so add the object to the appropriate child
			if (index != -1)
			{
				m_Children[index]->Add(item, x, y);
			}
		}
		else if(index != -1)
		{
			if (std::find(m_Data.begin(), m_Data.end(), item) == m_Data.end())
			{
				// This is a leaf node, so store the object in this node
				m_Data.push_back(item);
			}
		}
	}

	void Remove(T item, float x, float y)
	{
		if (IsLeaf())
		{
			// Check if the given point exists in this node
			auto iterator = std::find(m_Data.begin(), m_Data.end(), item);
			if (iterator != m_Data.end())
			{
				// The point exists in this node, so remove it
				m_Data.erase(iterator);
			}
		}
		else
		{
			// The point does not exist in this node, so check its children
			int index = FindIndex(x, y);
			if (index != -1 && m_Children[index] != nullptr)
			{
				m_Children[index]->Remove(item, x, y);
			}
		}
	}

private:

	AABB m_AABB;
	std::shared_ptr<QuadTreeNode> m_Children[4] = { nullptr, nullptr, nullptr, nullptr };
	std::vector<T> m_Data{};
};


//TODO figure out how to iterate over nodes with more than one entity and do collision checks
//TODO figure out how we can do boundry checks using the quadtrees border nodes
//TODO make entites able to overlap multiple nodes by passing their collider size instead of just position
//TODO ultimatley we only want to check collisions in nodes where there are more than one entites that have different tags, borders are a differnt case

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

	void Add(T item, float x, float y) { m_Root->Add(item, x, y); }
	void Remove(T item, float x, float y) { m_Root->Remove(item, x, y); }

	void Update(T item, float x, float y, float prevX, float prevY)
	{
		auto nextNode = m_Root->FindNode(x, y);
		if (nextNode == nullptr)
		{
			// the next position is outside the quadtree, remove the item form the last node it was in
			m_Root->Remove(item, prevX, prevY);
			return;
		}

		auto node = m_Root->FindNode(prevX, prevY);
		if (node == nullptr)
		{
			// The previous position of the object is not within the root node, so add it to the tree
			m_Root->Add(item, x, y);
			return;
		}
		
	

		// Check if the object has entered a new quadrant
		if (nextNode != node)
		{
			// The object has entered a new quadrant, so remove it from its current node and add it to the appropriate leaf node
			m_Root->Remove(item, prevX, prevY);
			m_Root->Add(item, x, y);
		}
	}

private:
	std::shared_ptr<QuadTreeNode<T>> m_Root;
};
