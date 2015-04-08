#pragma once

#include "Pathfinder.h"
#include <allegro5/allegro.h>

class GridGraph;
class GridVisualizer;
class GraphicsBuffer;
class PathfindingDebugContent;

class GridPathfinder:public Pathfinder
{
public:
	friend class PathfindingDebugContent;

	GridPathfinder( GridGraph* pGraph );
	virtual ~GridPathfinder();

	virtual Path* findPath( Node* pFrom, Node* pTo ) = 0;

	inline static void setPathColor(int i) { CurrentColor = i; }
	inline static void switchDrawVisited() { DrawVisited = !DrawVisited; }
	
#ifdef VISUALIZE_PATH
	//just for visualization
public:
	void drawVisualization( Grid* pGrid, GraphicsBuffer* pDest );
protected:
	std::vector<Node*> mVisitedNodes;
	Path* mpPath;
	GridVisualizer* mpVisualizer;
#endif

	ALLEGRO_COLOR mPathColor;
	
	static int CurrentColor;
	static bool DrawVisited;

	

	double mTimeElapsed;
};