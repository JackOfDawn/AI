#include "PortalPathfinder.h"
#include "PortalGraph.h"
#include "Path.h"
#include "NodeMetaData.h"
#include <map>
#include <list>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include "Connection.h"


PortalPathfinder::PortalPathfinder(PortalGraph* pGraph)
:Pathfinder(pGraph)
{

}

PortalPathfinder::~PortalPathfinder()
{

}

Path* PortalPathfinder::findPath(Node* pFromNode, Node* pToNode)
{

	//create initial node metadata
	NodeMetaData* v = new NodeMetaData();
	v->pCurrentNode = pFromNode;
	v->distance = 0;
	v->scanned = false;
	v->pPreviousNode = NULL;

	NodeMetaData* finalNode = NULL;

	//create new patth
	Path* pPath = new Path();

	std::map<const int, NodeMetaData*> nodeStructs = std::map<const int, NodeMetaData*>();
	std::vector <NodeMetaData*> pathQueue;
	//std::priority_queue<NodeMetaData*, std::vector<NodeMetaData*>, DijkstraComparison> pathQueue;

	//pathQueue.push(v);
	pathQueue.push_back(v);
	nodeStructs.insert(std::pair<const int, NodeMetaData*>(v->pCurrentNode->getId(), v));

	while (!pathQueue.empty())
	{
		//NodeMetaData* u = pathQueue.top();
		//pathQueue.pop();
		int smallestDistance = INT_MAX;
		int chosenNode = 0;
		for (size_t i = 0; i < pathQueue.size(); i++)
		{
			if (pathQueue[i]->distance < smallestDistance)
			{
				chosenNode = i;
				smallestDistance = pathQueue[i]->distance;
			}
		}

		NodeMetaData* u = pathQueue[chosenNode];
		pathQueue.erase(pathQueue.begin() + chosenNode);
		

		//if it is the end node, break out
		if (u->pCurrentNode->getId() == pToNode->getId())
		{
			finalNode = u;
			break;
		}

		u->scanned = true;


		std::vector<Connection*> connections = mpGraph->getConnections(u->pCurrentNode->getId());

		for each (Connection* connection in connections)
		{
			if (nodeStructs[connection->getToNode()->getId()] != NULL) //check if struct exists yet
			{
				//check if it hace already been scanned
				if (nodeStructs[connection->getToNode()->getId()]->scanned)
				{
					continue; //if scanned move on
				}
				else
				{
					v = nodeStructs[connection->getToNode()->getId()];
				}
			}
			else //make a new nodestruct
			{
				v = new NodeMetaData();
				v->pCurrentNode = connection->getToNode();
				v->pPreviousNode = NULL;
				v->scanned = false;
				v->distance = (float)INT_MAX;

				//add it to the map;
				nodeStructs[connection->getToNode()->getId()] = v;
			}

			float altDistance = u->distance + connection->getCost();

			if (altDistance < v->distance)
			{
				v->distance = altDistance;
				v->pPreviousNode = u;
				pathQueue.push_back(v);
			}
		}
	}

	//add the nodes to the path
	//retrace fron the final not to a stack
	NodeMetaData* tmpNode;
	std::stack<Node*> reversePath;
	tmpNode = finalNode;

	while (tmpNode != NULL)
	{
		reversePath.push(tmpNode->pCurrentNode);
		tmpNode = tmpNode->pPreviousNode;
	}


	//create a path of one
	if (finalNode == NULL)
	{
		pPath->addNode(pFromNode);
	}
	while (reversePath.empty() == false)
	{
		pPath->addNode(reversePath.top());
		reversePath.pop();
	}
	//once the path is found delete all of the node structs

	std::map<const int, NodeMetaData*>::iterator iter;
	for (iter = nodeStructs.begin(); iter != nodeStructs.end(); ++iter)
	{
		NodeMetaData* pNMD = iter->second;
		delete pNMD;
	}
	nodeStructs.clear();

	return pPath;
}


