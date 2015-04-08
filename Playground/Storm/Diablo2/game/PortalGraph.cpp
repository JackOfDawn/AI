#include "PortalGraph.h"
#include "Connection.h"
#include "Node.h"
#include <Game.h>
#include <vector>
#include "MapDefines.h"
#include <Defines.h>

PortalGraph::PortalGraph(std::vector<PortalConnection>* pPortalConnections)
:Graph()
{
	mpPortalConnections = pPortalConnections;
}


void PortalGraph::newConnections(std::vector<PortalConnection>* pPortalConnections)
{
	mpPortalConnections = pPortalConnections;
	init();
}
void PortalGraph::init()
{
	//size the node vector

	mNodes.resize(MAX_PORTALS + 2, NULL); //set the size two the number of portals pplus the start and end nodes

	//go through and make nodes for all the "portals" and add them to the node vector
	for (int i = 0; i < mNodes.size(); i++)
	{
		Node* pNode = new Node(i);
		mNodes[i] = pNode;
	}

	//go through the portalconnections and create connections for each
	for (int i = 0; i < mNodes.size(); i++)
	{
		//get node for this location
		Node* pFromNode = mNodes[i];

		//creat a vector of connections
		std::vector<Connection*> connections;

		//go through all connections for the specific portal
		for (size_t j = 0; j < mpPortalConnections[i].size(); j++)
		{
			int index = mpPortalConnections[i].at(j).connectedID % MIN_PORTAL_ID; //get the index of a connected node;

			Node* pToNode = mNodes[index]; //find to node

			int cost = mpPortalConnections[i].at(j).cost;
			Connection* pConnection = new Connection(pFromNode, pToNode, cost); //create a connection

			mConnections.push_back(pConnection);
			connections.push_back(pConnection);
		}

		mConnectionMap[i] = connections;

	}
}