#pragma once

#include "GridPathfinder.h"
#include <vector>
#include "NodeMetaData.h"

class Path;
class Graph;
class GraphicsBuffer;
class Grid;

struct AStarComparison
{
	bool operator() (const NodeMetaData* lhs, const NodeMetaData* rhs)
	{
		return lhs->cost > rhs->cost;
	}
};

class AStarPathfinder : public GridPathfinder
{
public:
	AStarPathfinder(Graph * pGraph);
	~AStarPathfinder();

	Path* findPath(Node* pFrom, Node* pTo);

private:

	float getHDistance(Node* pFrom, Node* pTo);
};