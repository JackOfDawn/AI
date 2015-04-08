#include "MapHolder.h"
#include <Map.h>
#include "GridGraph.h"
#include "AStarPathFinder.h"
#include "GridPathfinder.h"
#include <GridVisualizer.h>
#include <Grid.h>
#include "GiveUnitPathMessage.h"
#include "GameMessage.h"
#include "GameApp.h"
#include "GameMessageManager.h"
#include "SetEndPointMessage.h"


MapHolder* gpMapHolder = NULL;

MapHolder::MapHolder()
{
	mpMap = NULL;
	mpGridGraph = NULL;

	mpPortalGraph = NULL;
	mpPortalPathfinder = NULL;
	mpPortalPath = NULL;

	mEndPoint = EndPoint();
	mStartPoint = EndPoint();
	mGrueSpawns = std::vector<SpawnInfo>();
	mCandySpawns = std::vector<SpawnInfo>();
	mMapIDs = std::vector<int>();
}

MapHolder::~MapHolder()
{
	clear();
}

void MapHolder::clear()
{
	//clear maps
	std::map<IDType, Map*>::iterator iter;
	for (iter = mMapList.begin(); iter != mMapList.end(); ++iter)
	{
		Map* pMap = iter->second;
		delete pMap;
	}
	mpMap = NULL;

	//clear Pathfinders
	std::map<IDType, GridPathfinder*>::iterator iter2;
	for (iter2 = mPathfinders.begin(); iter2 != mPathfinders.end(); ++iter2)
	{
		GridPathfinder* pPathfinder = iter2->second;
		delete pPathfinder;
	}
	

	//clear gridgraphs
	std::map<IDType, GridGraph*>::iterator iter3;
	for (iter3 = mGridGraphs.begin(); iter3 != mGridGraphs.end(); ++iter3)
	{
		GridGraph* pGridGraph = iter3->second;
		delete pGridGraph;
	}
	mpGridGraph = NULL;

	delete mpPortalGraph;
	mpPortalGraph = NULL;

	delete mpPortalPath;
	delete mpPortalPathfinder;
	delete mpPortalGraph;
}

void MapHolder::setup()
{
	//iterate through the maplist to set up the pathfinders and graphs
	std::map<IDType, Map*>::iterator iter;
	for (iter = mMapList.begin(); iter != mMapList.end(); ++iter)
	{
		Map* pMap = iter->second;

		mGridGraphs[pMap->getID()] = new GridGraph(pMap->getGrid());
		mGridGraphs[pMap->getID()]->init();
		mPathfinders[pMap->getID()] = new AStarPathfinder(mGridGraphs[pMap->getID()]);
	}
	mpMap = mMapList[1];
	mpGridGraph = mGridGraphs[mpMap->getID()];
	

	setBasePortalConnections();
	mpPortalGraph = new PortalGraph(mBasePortalConnections);
	mpPortalPathfinder = new PortalPathfinder(mpPortalGraph);
}

void MapHolder::setBasePortalConnections()
{
	std::map<IDType, Map*>::iterator iter;
	for (iter = mMapList.begin(); iter != mMapList.end(); ++iter)
	{
		//get a map
		Map* pMap = iter->second;
		int mapID = pMap->getID();
		//get the links
		std::vector<PortalLinks> links = pMap->getLinks();

		for (size_t i = 0; i < links.size(); i++) //go through each link
		{
			int id = links[i].portalID;
			//get the portal ID mod 100
			int pos = id % MIN_PORTAL_ID;
			Node* pFromNode = mGridGraphs[mapID]->getNode(links[i].portalIndex);

			for ( size_t j = 0; j < links.size(); j++) //go through the other links
			{
				if (j != i) //if it isn't the same link
				{
					Node* pToNode = mGridGraphs[mapID]->getNode(links[j].portalIndex);
					mPathfinders[mapID]->findPath(pFromNode, pToNode);
					int cost = mPathfinders[mapID]->getPathSize();
					
					mPathfinders[mapID]->clearPath();
					PortalConnection newConnection = PortalConnection(id, links[j].portalID, cost); //need portalID, connection ID, and cost;
					mBasePortalConnections[pos].push_back(newConnection);
				}
			}
		}
	}
}

void MapHolder::setStartEndPortalConnections()
{
	std::map<IDType, Map*>::iterator iter;
	for (iter = mMapList.begin(); iter != mMapList.end(); ++iter)
	{
		//get a map
		Map* pMap = iter->second;
		mPathfinders[pMap->getID()]->clearPath();
	}
	for (size_t i = 0; i < MAX_PORTALS + 2; i++)
	{
		mStartEndPortalConnections[i].clear();
	}
	//set the SaEportalconnections to the base
	{
		for (size_t i = 0; i < MAX_PORTALS; i++) //Go through the base list
		{
			for each (PortalConnection connection in mBasePortalConnections[i]) //push over each connection
			{
				mStartEndPortalConnections[i].push_back(connection);
			}
		}
	}
	addStartPoint();
	addEndPoint();

	delete mpPortalGraph;
	mpPortalGraph = new PortalGraph(mStartEndPortalConnections);
	mpPortalGraph->init();
	delete mpPortalPathfinder;
	mpPortalPathfinder = new PortalPathfinder(mpPortalGraph);
}

void MapHolder::addEndPoint()
{
	std::vector<PortalLinks> links = mMapList[mEndPoint.mapID]->getLinks(); // get the links
	Node* pToNode = mGridGraphs[mEndPoint.mapID]->getNode(mEndPoint.index); //get the node for the start pos
	for (size_t i = 0; i < links.size(); i++)
	{
		Node* pFromNode = mGridGraphs[mEndPoint.mapID]->getNode(links[i].portalIndex);

		mPathfinders[mEndPoint.mapID]->findPath(pFromNode, pToNode);
		int cost = mPathfinders[mEndPoint.mapID]->getPathSize();
		mPathfinders[mEndPoint.mapID]->clearPath();

		//update starts connection to a portal
		PortalConnection newConnection = PortalConnection(links[i].portalID, mEndPoint.portalID, cost); //need portalID, connection ID, and cost;
		mStartEndPortalConnections[links[i].portalID % MIN_PORTAL_ID].push_back(newConnection);

		//update portal connection to start?
	}
}

void MapHolder::addStartPoint()
{
	//Add the start
	std::vector<PortalLinks> links = mMapList[mStartPoint.mapID]->getLinks(); // get the links
	Node* pFromNode = mGridGraphs[mStartPoint.mapID]->getNode(mStartPoint.index); //get the node for the start pos
	for (size_t i = 0; i < links.size(); i++)
	{
		Node* pToNode = mGridGraphs[mStartPoint.mapID]->getNode(links[i].portalIndex);

		mPathfinders[mStartPoint.mapID]->findPath(pFromNode, pToNode);
		int cost = mPathfinders[mStartPoint.mapID]->getPathSize();
		mPathfinders[mStartPoint.mapID]->clearPath();

		//update starts connection to a portal
		PortalConnection newConnection = PortalConnection(mStartPoint.portalID, links[i].portalID, cost); //need portalID, connection ID, and cost;
		mStartEndPortalConnections[START_ID % MIN_PORTAL_ID].push_back(newConnection);

		//update portal connection to start?
	}

	//check connection with the end point
	if (mStartPoint.mapID == mEndPoint.mapID)//same map
	{
		Node* pToNode = mGridGraphs[mStartPoint.mapID]->getNode(mEndPoint.index);

		mPathfinders[mStartPoint.mapID]->findPath(pFromNode, pToNode);
		int cost = mPathfinders[mStartPoint.mapID]->getPathSize();
		mPathfinders[mStartPoint.mapID]->clearPath();

		//update starts connection to a portal
		PortalConnection newConnection = PortalConnection(mStartPoint.portalID, mEndPoint.portalID, cost); //need portalID, connection ID, and cost;
		mStartEndPortalConnections[START_ID % MIN_PORTAL_ID].push_back(newConnection);
	}
}

void MapHolder::loadMaps(std::string fileName)
{
	std::ifstream theStream(fileName);
	std::string junk;

	int numMaps;
	theStream >> numMaps;
	std::getline(theStream, junk);

	Map* tmpMap;
	for (int i = 0; i < numMaps; i++)
	{
		tmpMap = new Map(); //create new map
		tmpMap->loadGrid(theStream); //load data into map
		addLinks(tmpMap->getLinks()); //get portal information
		mMapList[tmpMap->getID()] = tmpMap;//add map to the mapList
		mMapIDs.push_back(tmpMap->getID());
	}

	//Store Grue Spawns
	//Store MightyCandySpawns
	//Store Player Spawn Point

	std::map<IDType, Map*>::iterator iter;
	for (iter = mMapList.begin(); iter != mMapList.end(); ++iter)
	{
		Map* tmpMap = iter->second;
		std::vector<int> gruevalues = tmpMap->getGrues();
		for each (int grue in gruevalues)
		{
			SpawnInfo tmpSpawn = SpawnInfo();
			tmpSpawn.mapID = tmpMap->getID();
			tmpSpawn.location = grue;
			mGrueSpawns.push_back(tmpSpawn);
		}

		std::vector<int> candyValues = tmpMap->getCandy();
		for each (int candy in candyValues)
		{
			SpawnInfo tmpSpawn = SpawnInfo();
			tmpSpawn.mapID = tmpMap->getID();
			tmpSpawn.location = candy;
			mCandySpawns.push_back(tmpSpawn);
		}

		if (tmpMap->getPlayer() != INVALID_ID)
		{
			mPlayerSpawn.location = tmpMap->getPlayer();
			mPlayerSpawn.mapID = tmpMap->getID();
		}

	}



	setup();
}

void MapHolder::addLinks(std::vector<PortalLinks> links)
{
	for each (PortalLinks link in links)
	{
		int iter = link.portalID % MIN_PORTAL_ID; //get the portal spot

		if (mPortals[iter].map2ID == INVALID_ID)
		{
			mPortals[iter].map2ID = link.map2ID;
			mPortals[iter].portalIndexMap1 = link.portalIndex;
		}
		else if (mPortals[iter].map1ID == INVALID_ID)
		{
			mPortals[iter].map1ID = link.map2ID;
			mPortals[iter].portalIndexMap2 = link.portalIndex;
		}
	}
}

void MapHolder::checkMouseClick(int x, int y, bool leftClick)
{
	//check if click is a portal
	int value = mpMap->getGrid()->getValueAtPixelXY(x, y);

	if (value >= MIN_PORTAL_ID)
	{
		int newMapID = getPortalLinkID(value);
		changeMapToID(newMapID);
	}
}

PathingInfo MapHolder::generatePath(EndPoint start, EndPoint end)
{
	PathingInfo pathInfo;
	mEndPoint = end;
	mStartPoint = start;
	mStartPoint.portalID = START_ID;
	mEndPoint.portalID = END_ID;

	//UPDATE TRUE PORTAL CONNECTIONS
	setStartEndPortalConnections();
	//FIND THE PATH
	Node* pFrom = mpPortalGraph->getNode(START_ID % MIN_PORTAL_ID);
	Node* pTo = mpPortalGraph->getNode(END_ID % MIN_PORTAL_ID);

	delete mpPortalPath;
	mpPortalPath = mpPortalPathfinder->findPath(pFrom, pTo);

	//modify the generate paths function
	pathInfo = generatePaths();

	return pathInfo;
}

PathingInfo MapHolder::generatePaths()
{
	Node* pFrom;
	Node* pTo;
	mCurrPathingMap = 0;
	mReversed = false;
	mPathOfMaps.clear();
	mPathOfMaps.shrink_to_fit();

	std::vector<int>mapPath = std::vector<int>();
	std::vector<std::pair<Node*, Node*>>nodePairs = std::vector<std::pair<Node*, Node*>>();


	if (mpPortalPath->getNumNodes() == 2)
	{
		pFrom = mGridGraphs[mStartPoint.mapID]->getNode(mStartPoint.index);
		pTo = mGridGraphs[mStartPoint.mapID]->getNode(mEndPoint.index);
		mapPath.push_back(mStartPoint.mapID);
		nodePairs.push_back(std::make_pair(pFrom, pTo));
	}
	else if (mpPortalPath->getNumNodes() <= 1)
	{
		//uhhh later
	}
	else
	{
		int mapID = mStartPoint.mapID;
		pFrom = mGridGraphs[mapID]->getNode(mStartPoint.index);
		//mPathOfMaps.push_back(mapID);
		mapPath.push_back(mapID);
		int portalID;
		int index;
		int nextMap;
		int nextIndex;
		int nextPortalID = mpPortalPath->peekNode(1)->getId() + MIN_PORTAL_ID;
		std::vector<PortalLinks> links = mMapList[mapID]->getLinks();// .size();
		for (size_t i = 0; i < links.size(); i++)
		{
			if (links[i].portalID == nextPortalID)
			{
				nextIndex = links[i].portalIndex;
				nextMap = links[i].map2ID;
				break;
			}
		}
		pTo = mGridGraphs[mapID]->getNode(nextIndex);
		//mPathfinders[mapID]->findPath(pFrom, pTo);
		nodePairs.push_back(std::make_pair(pFrom, pTo));

		index = nextIndex;
		portalID = nextPortalID;
		mapID = nextMap;
		mapPath.push_back(mapID);
		//mPathOfMaps.push_back(mapID);
		//iter over most of the files
		for (int j = 1; j < mpPortalPath->getNumNodes() - 2; j++)
		{
			std::vector<PortalLinks> links = mMapList[mapID]->getLinks();
			for (size_t k = 0; k < links.size(); k++) //loop to update old index
			{
				if (links[k].portalID == portalID)
				{
					index = links[k].portalIndex;
					break;
				}
			}

			pFrom = mGridGraphs[mapID]->getNode(index);
			int nextPortalID = mpPortalPath->peekNode(j + 1)->getId() + MIN_PORTAL_ID;

			for (size_t k = 0; k < links.size(); k++)//loop to get new
			{
				if (links[k].portalID == nextPortalID)
				{
					nextIndex = links[k].portalIndex;
					nextMap = links[k].map2ID;
					break;
				}
			}
			pTo = mGridGraphs[mapID]->getNode(nextIndex);
			//mPathfinders[mapID]->findPath(pFrom, pTo);
			nodePairs.push_back(std::make_pair(pFrom, pTo));

			index = nextIndex;
			portalID = nextPortalID;
			mapID = nextMap;
			//mPathOfMaps.push_back(mapID);
			mapPath.push_back(mapID);
		}

		links = mMapList[mapID]->getLinks();
		for (size_t k = 0; k < links.size(); k++) //loop to update old index
		{
			if (links[k].portalID == portalID)
			{
				index = links[k].portalIndex;
				break;
			}
		}
		pFrom = mGridGraphs[mapID]->getNode(index);
		pTo = mGridGraphs[mapID]->getNode(mEndPoint.index);
		//mPathfinders[mapID]->findPath(pFrom, pTo);
		nodePairs.push_back(std::make_pair(pFrom, pTo));
	}

	//Set up the pathing info struct
	PathingInfo pathInfo = PathingInfo();
	pathInfo.mapIDs = mapPath;
	pathInfo.nodePairs = nodePairs;

	return pathInfo;

}

EndPoint MapHolder::generateValidTarget()
{
	EndPoint newTarget;
	//Choose a random map
	int mapID = mMapIDs[rand() % mMapIDs.size()];
	//Choose a none wall location on the map
	int index = mMapList[mapID]->getGrid()->getRandomEmpty();

	newTarget.portalID = END_ID;
	newTarget.mapID = mapID;
	newTarget.index = index;
	
	return newTarget;
}

int MapHolder::getCorrectIndex(int portalIndex, int mapID)
{
	if (mPortals[portalIndex].map1ID == mapID)
	{
		return mPortals[portalIndex].portalIndexMap1;
	}
	else if(mPortals[portalIndex].map2ID == mapID)
	{
		return mPortals[portalIndex].portalIndexMap2;
	}

	return 0;
}

EndPoint MapHolder::setPathPoint(int x, int y)
{
	int indexOnMap = mpMap->getGrid()->getSquareIndexFromPixelXY(x, y);
	int value = mpMap->getGrid()->getValueAtIndex(indexOnMap);
	EndPoint newEndPoint = EndPoint();

	if (value != WALL_BLOCK_ID && value < MIN_PORTAL_ID)
	{
		newEndPoint.index = indexOnMap;
		newEndPoint.mapID = mpMap->getID();
		newEndPoint.portalID = END_ID;
	}
	return newEndPoint;
}

void MapHolder::changeMapToID(int mapID)
{
	mpMap = mMapList[mapID];
	mpGridGraph = mGridGraphs[mapID];

	
}

int MapHolder::getPortalLinkID(int value)
{
	PortalInfo portal = mPortals[value % MIN_PORTAL_ID];

	if (portal.map1ID == mpMap->getID())
	{
		return portal.map2ID;
	}
	else
	{
		return portal.map1ID;
	}
}

bool MapHolder::isValidPathMap()
{
	if (mPathOfMaps.size() > 0)
	{
		if (mPathOfMaps[mCurrPathingMap] == mpMap->getID()) // if the ID's are the same, we are on the correct map
		{
			return true;
		}
	}

	return false;
}

void MapHolder::checkPathMaps()
{
	if (!mReversed)
	{
		if (mPathOfMaps.size() - 1 == mCurrPathingMap)
		{
			mReversed = true;
			
		}
		else
		{
			mCurrPathingMap++;
			giveTheVectorPath();
			changeMapToID(mPathOfMaps[mCurrPathingMap]);
		}
	}
	else
	{
		if (0 == mCurrPathingMap)
		{
			mReversed = false;
			
		}
		else
		{
			mCurrPathingMap--;
			giveTheVectorPath();
			changeMapToID(mPathOfMaps[mCurrPathingMap]);
		}
	}

}

void MapHolder::giveTheVectorPath()
{
	int mapID = mPathOfMaps[mCurrPathingMap];
	std::vector<Vector2D> vectorPath = mGridGraphs[mapID]->convertPathNodesToVectors(mPathfinders[mapID]->returnPath());
	GameMessage* pMessage = new GiveUnitPathMessage(vectorPath, mReversed);
	dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
}

GridPathfinder* MapHolder::getCurrentGridPathfinder()
{
	return mPathfinders[mpMap->getID()];
}

SpawnInfo MapHolder::getGrueSpawn(int idx)
{
	return mGrueSpawns[idx];
}
void MapHolder::generatePlayerEndPoint(int x, int y)
{
	int indexOnMap = mpMap->getGrid()->getSquareIndexFromPixelXY(x, y);
	int value = mpMap->getGrid()->getValueAtIndex(indexOnMap);
	EndPoint newEndPoint = EndPoint();

	if (value != WALL_BLOCK_ID && value < MIN_PORTAL_ID)
	{
		newEndPoint.index = indexOnMap;
		newEndPoint.mapID = mpMap->getID();
		newEndPoint.portalID = END_ID;
	}


	//Send a message
	if (newEndPoint.isValid())
	{
		GameMessage* pMessage = new SetEndPointMessage(newEndPoint);
		dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}
}
