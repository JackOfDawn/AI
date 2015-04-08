#include "GridGraph.h"
#include "Connection.h"
#include "Node.h"
#include "Grid.h"
#include "Game.h"
#include <vector>
#include "Vector2D.h"
#include "Path.h"

GridGraph::GridGraph( Grid* pGrid )
:Graph()
,mpGrid(pGrid)
{
}

GridGraph::~GridGraph()
{
}

void GridGraph::init()
{
	if( mpGrid != NULL )
	{
		int numGridIndices = mpGrid->getGridWidth() * mpGrid->getGridHeight();

		//size the node vector
		mNodes.resize( numGridIndices, NULL );
		
		//go through all elements of the grid and make Nodes for them - also add them to Node vector
		for( int i=0; i<numGridIndices; i++ )
		{
			//create a Node for this location
			Node* pNode = new Node( i );
			//add to NodeVector
			mNodes[i] = pNode;
		}

		//go through all elements of the grid - get adjacencies and create connections for each
		for( int i=0; i<numGridIndices; i++ )
		{
			//get the Node for this location
			Node* pFromNode = mNodes[i];

			//get all the adjacencies
			std::vector<int> adjacencies = mpGrid->getAdjacentIndices( i );

			//create a vector of Connections
			std::vector<Connection*> connections;

			//go through all adjacent indices, get the nodes for them, create connections, and add to connections map
			for( unsigned int adjIndex = 0; adjIndex < adjacencies.size(); adjIndex++ )
			{
				//check for blocking terrain
				if( mpGrid->getValueAtIndex(adjacencies[adjIndex]) != BLOCKING_VALUE )
				{
					Node* pToNode = mNodes[ adjacencies[adjIndex] ];//find to node

					Connection* pConnection = new Connection( pFromNode, pToNode, 1.0f );//create a connection

					//add connection to connection vectors
					mConnections.push_back( pConnection );
					connections.push_back( pConnection );
				}
			}

			//put temp connection vector into map
			mConnectionMap[i] = connections;
		}
	}
}


float GridGraph::findDistanceSq(Node* pFrom, Node* pTo)
{
	float distance;
	Vector2D fromPos = mpGrid->getULCornerOfSquare(pFrom->getId());
	Vector2D toPos = mpGrid->getULCornerOfSquare(pTo->getId());

	distance = Vector2D::distanceSq(fromPos, toPos);

	return distance;
}
float GridGraph::findDistanceSq(const NODE_ID& fromID, const NODE_ID& toID)
{
	float distance;
	Vector2D fromPos = mpGrid->getULCornerOfSquare(fromID);
	Vector2D toPos = mpGrid->getULCornerOfSquare(toID);

	distance = Vector2D::distanceSq(fromPos, toPos);

	return distance;
}

int GridGraph::getSquareSize()
{
	return mpGrid->getSquareSize();
}

Vector2D GridGraph::getLocation(Node* pNode)
{
	return mpGrid->getULCornerOfSquare(pNode->getId());
}

std::vector<Vector2D> GridGraph::convertPathNodesToVectors(Path* pPath)
{
	std::vector<Vector2D> vector2DPath;
	if (pPath != NULL)
	{
		
		for (int i = 0; i < pPath->getNumNodes(); i++)
		{
			Vector2D tmpLoc = getLocation(pPath->peekNode(i)); //gets upper left pos
			tmpLoc.setXY(tmpLoc.getX() + mpGrid->getSquareSize() / 2, tmpLoc.getY() + mpGrid->getSquareSize() / 2);//convert to center pos
			vector2DPath.push_back(tmpLoc);
		}
	}

	return vector2DPath;

}
