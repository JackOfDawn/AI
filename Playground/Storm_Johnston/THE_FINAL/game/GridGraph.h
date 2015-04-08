#pragma once

#include "Graph.h"
#include "Vector2D.h"

class Grid;
class Path;

class GridGraph:public Graph
{
public:
	GridGraph( Grid* pGrid );
	virtual ~GridGraph();

	void setGrid( Grid* pGrid ) { mpGrid = pGrid; };
	void init();

	float findDistanceSq(Node* pfrom, Node* pTo);
	float findDistanceSq(const NODE_ID& fromID, const NODE_ID& toID);
	int getSquareSize();
	Vector2D getLocation(Node* pNode);

	std::vector<Vector2D> convertPathNodesToVectors(Path* path);

private:
	Grid* mpGrid;
};