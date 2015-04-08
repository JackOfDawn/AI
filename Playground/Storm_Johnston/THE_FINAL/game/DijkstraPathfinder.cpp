#include "DijkstraPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include <Game.h>
#include <PerformanceTracker.h>
#include <vector>
#include <algorithm>
#include <list>
#include <stack>
#include <map>
#include <queue>          // std::priority_queue

using namespace std;

DijkstraPathfinder::DijkstraPathfinder(Graph * pGraph)
: GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
}

DijkstraPathfinder::~DijkstraPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path* DijkstraPathfinder::findPath(Node* pFrom, Node* pTo)
{
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");
	//allocate nodes to visit list and place starting node in it

	//create initialNode;
	NodeMetaData* v = new NodeMetaData();
	v->pCurrentNode = pFrom;
	v->distance = 0;
	v->scanned = false;
	v->pPreviousNode = NULL;
	
	NodeMetaData* finalNode = NULL;

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear();
	mVisitedNodes.push_back(pFrom);
#endif

	//create Path
	Path* pPath = new Path();

	//Things happen
	std::map<const int, NodeMetaData*> nodeStructs = std::map<const int, NodeMetaData*>();
	//std::vector <NodeMetaData*> pathQueue;
	std::priority_queue<NodeMetaData*, vector<NodeMetaData*>, DijkstraComparison> pathQueue;

	//pathQueue.push_back(v);
	pathQueue.push(v);
	nodeStructs.insert(pair<const int, NodeMetaData*>(v->pCurrentNode->getId(), v));

	while (pathQueue.empty() == false)
	{

		//get the data with smallest size distance
		/*
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
		*/
		NodeMetaData* u = pathQueue.top();
		pathQueue.pop();

#ifdef VISUALIZE_PATH
		mVisitedNodes.push_back(u->pCurrentNode);
#endif
		
		if (u->pCurrentNode->getId() == pTo->getId())
		{
			finalNode = u;
			break;
		}
		//Set the current node to scanned
		u->scanned = true;


		//get the connections of the current node
		vector<Connection*> connections = mpGraph->getConnections(u->pCurrentNode->getId());
		
		for each (Connection* connection in connections)
		{
			//NodeMetaData* v;
			//check if v has already been scanned
			if (nodeStructs[connection->getToNode()->getId()] != NULL) //check if struct exists yet
			{
				//if it exists checked if scanned
				if (nodeStructs[connection->getToNode()->getId()]->scanned)
				{
					continue; //if scanned move on
				}
				else //if it has not been scanned, set it to v
				{
					v = nodeStructs[connection->getToNode()->getId()];
				}
			}
			else //currently is no meta data for current node
			{
				//create new node
				v = new NodeMetaData();
				v->pCurrentNode = connection->getToNode();
				v->pPreviousNode = NULL;
				v->scanned = false;
				v->distance = (float)INT_MAX;
				//add it to the map
				nodeStructs[connection->getToNode()->getId()] = v;
				//nodeStructs.insert(pair<const int, NodeMetaData*>(v->pCurrentNode->getId(), v));
			}

			float altDistance = u->distance + connection->getCost();
			
			if (altDistance < v->distance)
			{
				v->distance = altDistance;
				v->pPreviousNode = u;
				//pathQueue.push_back(v);
				pathQueue.push(v);
			}


		}
	}
	
	//add the nodes to the path
	//retrace from final node to a stack
	NodeMetaData* tmpNode;
	std::stack<Node*> reversePath;
	tmpNode = finalNode;

	
	while (tmpNode != NULL)
	{
		reversePath.push(tmpNode->pCurrentNode);
		tmpNode = tmpNode->pPreviousNode;
	}

	if (finalNode == NULL)
	{
		pPath->addNode(pFrom);
	}
	while (reversePath.empty() == false)
	{
		pPath->addNode(reversePath.top());
		reversePath.pop();
	}
	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

	//once the path is found delete all of the node structs

	map<const int, NodeMetaData*>::iterator iter;
	for (iter = nodeStructs.begin(); iter != nodeStructs.end(); ++iter)
	{
		NodeMetaData* pNMD = iter->second;
		delete pNMD;
	}
	nodeStructs.clear();

#ifdef VISUALIZE_PATH
	mpPath = pPath;
#endif
	return pPath;
}

