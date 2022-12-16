#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <memory>
#include <functional>
#include "AABB.h"
#include "../ECS/ECSCore.h"


template<typename T>
class QuadTreeNode
{
public:
	QuadTreeNode(float min_x, float min_y, float max_x, float max_y)
		: m_AABB(min_x, min_y, max_x, max_y)
	{
		m_Data.reserve(MAX_ENTITIES * 0.01);
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

	std::vector<int> FindIndices(const AABB& itemCol) const
	{
		std::vector<int> indices{};

		float cx = (m_AABB.min_x + m_AABB.max_x) / 2;
		float cy = (m_AABB.min_y + m_AABB.max_y) / 2;

		if (itemCol.IsIntersecting(AABB(m_AABB.min_x, m_AABB.min_y, cx, cy))) { indices.push_back(0); }
		else if (itemCol.IsIntersecting(AABB(cx, m_AABB.min_y, m_AABB.max_x, cy))) { indices.push_back(1); }
		else if (itemCol.IsIntersecting(AABB(m_AABB.min_x, cy, cx, m_AABB.max_y))) { indices.push_back(2); }
		else if (itemCol.IsIntersecting(AABB(cx, cy, m_AABB.max_x, m_AABB.max_y))) { indices.push_back(3); }

		return indices;
	}

	QuadTreeNode<T>* FindNode(float x, float y)
	{
		if (!IsLeaf())
		{
			// This node is not a leaf, so continue traversing further
			int index = FindIndex(x, y);
			if (index != -1)
			{
				return m_Children[index]->FindNode(x, y);
			}
		}
		else
		{
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

	std::vector<QuadTreeNode<T>*> FindNodes(const AABB& itemCol)
	{
		std::vector<QuadTreeNode<T>*> nodes{};

		if (!IsLeaf())
		{
			// This node is not a leaf, so continue traversing further
			std::vector<int> indices = FindIndices(itemCol);
			auto size = indices.size();
			if (size != 0)
			{
				for (size_t i = 0; i < size; i++)
				{
					auto childNodes = m_Children[indices[i]]->FindNodes(itemCol);
					nodes.insert(nodes.end(), childNodes.begin(), childNodes.end());
				}
			}
		}
		else
		{
			
			if (itemCol.IsIntersecting(m_AABB))
			{
				nodes.push_back(this);
			}
		}
		return nodes;
	}

	void Add(T item, float x, float y)
	{
		int index = FindIndex(x, y);
		if (index != -1)
		{
			if (!IsLeaf())
			{
				// This node is not a leaf, so add the item to the appropriate child		
				m_Children[index]->Add(item, x, y);
			}
			else
			{
				if (std::find(m_Data.begin(), m_Data.end(), item) == m_Data.end())
				{
					// This is a leaf node, so store the item in this node
					m_Data.push_back(item);
				}
			}
		}
	}

	void Add(T item, const AABB& itemCol)
	{
		std::vector<int> indices = FindIndices(itemCol);
		auto size = indices.size();
		if (size != 0)
		{
			if (!IsLeaf())
			{
				// This node is not a leaf, so add the item to the appropriate children

				for (size_t i = 0; i < size; i++)
				{
					m_Children[indices[i]]->Add(item, itemCol);

				}

			}
			else
			{
				if (std::find(m_Data.begin(), m_Data.end(), item) == m_Data.end())
				{
					// This is a leaf node, so store the item in this node
					m_Data.push_back(item);
				}
			}
		}

	}

	void Remove(T item, float x, float y)
	{
		if (IsLeaf())
		{
			// Check if the given item exists in this node
			auto iterator = std::find(m_Data.begin(), m_Data.end(), item);
			if (iterator != m_Data.end())
			{
				// The item exists in this node, so remove it
				m_Data.erase(iterator);
			}
		}
		else
		{
			//we are not at leaf level, so travel further
			int index = FindIndex(x, y);
			if (index != -1)
			{
				m_Children[index]->Remove(item, x, y);
			}
		}
	}

	void Remove(T item, const AABB& itemCol)
	{
		if (IsLeaf())
		{
			// Check if the given item exists in this node
			auto iterator = std::find(m_Data.begin(), m_Data.end(), item);
			if (iterator != m_Data.end())
			{
				// The item exists in this node, so remove it
				m_Data.erase(iterator);
			}
		}
		else
		{
			//we are not at leaf level, so travel further
			std::vector<int> indices = FindIndices(itemCol);
			auto size = indices.size();
			if (size != 0) 
			{
				for (size_t i = 0; i < size; i++)
				{
					m_Children[indices[i]]->Remove(item, itemCol);

				}
			}
			
		}
	}

private:

	AABB m_AABB;
	std::shared_ptr<QuadTreeNode<T>> m_Children[4] = { nullptr, nullptr, nullptr, nullptr };
	std::vector<T> m_Data{};
};

//TODO make entites able to overlap multiple nodes by passing their collider size instead of just position

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
	void Add(T item, const AABB& itemCol) { m_Root->Add(item, itemCol); }

	void Remove(T item, float x, float y) { m_Root->Remove(item, x, y); }
	void Remove(T item, const AABB& itemCol) { m_Root->Remove(item, itemCol); }

	void Update(T item, float x, float y, float prevX, float prevY)
	{
		QuadTreeNode<T>* nextNode = nullptr;
		nextNode = m_Root->FindNode(x, y);
		if (nextNode == nullptr)
		{
			// the next position is outside the quadtree, remove the item form the last node it was in
			m_Root->Remove(item, prevX, prevY);
			return;
		}

		QuadTreeNode<T>* oldNode = nullptr;
		oldNode = m_Root->FindNode(prevX, prevY);
		if (oldNode == nullptr)
		{
			// The previous position of the object is not within the root node, so add it to the tree
			m_Root->Add(item, x, y);
			return;
		}

		// Check if the object has entered a new quadrant
		if (nextNode != oldNode)
		{
			// The object has entered a new quadrant, so remove it from its current node and add it to the appropriate leaf node
			m_Root->Remove(item, prevX, prevY);
			m_Root->Add(item, x, y);
		}
	}

	void Update(T item, const AABB& nextCol, const AABB& oldCol)
	{
		std::vector<QuadTreeNode<T>*> nextNodes{};
		nextNodes = m_Root->FindNodes(nextCol);
		auto nextNodesSize = nextNodes.size();
		if (nextNodesSize == 0)
		{
			// the next position is outside the quadtree, remove the item form the last node it was in
			m_Root->Remove(item, oldCol);
			return;
		}

		std::vector<QuadTreeNode<T>*> oldNodes{};
		oldNodes = m_Root->FindNodes(oldCol);
		auto oldNodesSize = oldNodes.size();
		if (oldNodesSize == 0)
		{
			// The previous position of the object is not within the root node, so add it to the tree
			m_Root->Add(item,nextCol);
			return;
		}


		// Check if the object has entered a new quadrant
		if (nextNodesSize == oldNodesSize)
		{
			int numMatchingElements = std::equal(nextNodes.begin(), nextNodes.end(), oldNodes.begin(), oldNodes.end()) ? nextNodes.size() : 0;
			if (numMatchingElements == nextNodesSize) { return; }
		}

		m_Root->Remove(item, oldCol);
		m_Root->Add(item, nextCol);
	}

private:
	std::shared_ptr<QuadTreeNode<T>> m_Root;
};
