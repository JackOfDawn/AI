#pragma once

#include "Graph.h"

struct PortalConnection
{
	PortalConnection(int portalID, int connection, int costValue)
	{
		ID = portalID;
		connectedID = connection;
		cost = costValue;
	}
	int ID;
	int connectedID;
	int cost;
};
class PortalGraph : public Graph
{
public:
	PortalGraph(std::vector<PortalConnection>* portalConnections);
	~PortalGraph(){};

	void newConnections(std::vector<PortalConnection>* portalConnections);
	void init();

private:
	std::vector<PortalConnection>* mpPortalConnections;
};