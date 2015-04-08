#pragma once

#include "Pathfinder.h"

class PortalGraph;

class PortalPathfinder : public Pathfinder
{
public:
	PortalPathfinder(PortalGraph* pGraph);
	~PortalPathfinder();

	Path* findPath(Node* pFrom, Node* pTo);

};