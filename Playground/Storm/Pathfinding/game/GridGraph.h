#pragma once

#include "Graph.h"

class Grid;

class GridGraph:public Graph
{
public:
	GridGraph( Grid* pGrid );
	virtual ~GridGraph();

	void setGrid( Grid* pGrid ) { mpGrid = pGrid; };
	void init();

	float findDistanceSq(Node* pfrom, Node* pTo);
	float findDistanceSq(const NODE_ID& fromID, const NODE_ID& toID);

private:
	Grid* mpGrid;
};