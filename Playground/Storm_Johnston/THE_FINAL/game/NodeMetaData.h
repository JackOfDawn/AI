#pragma once

#include "Node.h"

struct NodeMetaData
{
	Node* pCurrentNode;
	NodeMetaData* pPreviousNode;
	bool scanned;
	float distance;
	float hDistance;
	float cost;

	void calcCost()
	{
		cost = distance + hDistance;
	};
};

struct DijkstraComparison
{
	bool operator() (const NodeMetaData* lhs, const NodeMetaData* rhs)
	{
		return lhs->distance > rhs->distance;
	}
};
