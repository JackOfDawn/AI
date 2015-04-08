#pragma once

#include <PortalInformation.h>
#include "Path.h"
#include "PortalPathfinder.h"
#include <vector>
#include <fstream>
#include <map>
#include "Trackable.h"
#include <Defines.h>
#include "PortalGraph.h"
#include "PathingInfo.h"
#include <Map.h>

class GridGraph;
class GridPathfinder;

class MapHolder;
extern MapHolder* gpMapHolder;

class MapHolder : public Trackable
{
public:

	MapHolder();
	~MapHolder();

	inline Map* getCurrentMap() { return mpMap; };
	inline Map* getMap(int ID) { return mMapList[ID]; }
	inline GridGraph* getGridGraph(int ID) { return mGridGraphs[ID]; }
	inline GridGraph* getCurrentGridGraph() { return mpGridGraph; };
	GridPathfinder* getCurrentGridPathfinder();
	void generatePlayerEndPoint(int x, int y);


	void loadMaps(std::string fileName);
	void clear();
	void checkMouseClick(int x, int y, bool leftClick);
	EndPoint setPathPoint(int x, int y);


	//void changeMaps(IDType mapID);
	bool isValidPathMap();
	
	void changeMapToID(int mapID);

	SpawnInfo getGrueSpawn(int idx);
	inline std::vector<SpawnInfo> getGrueSpawns() { return mGrueSpawns; };
	inline SpawnInfo getPlayerSpawn() { return mPlayerSpawn; };
	inline std::vector<SpawnInfo> getCandySpawns(){ return mCandySpawns; }

	void giveTheVectorPath();
	void updateMapFromUnit();

	void checkPathMaps();

	EndPoint generateValidTarget();
	PathingInfo generatePath(EndPoint start, EndPoint end);

private:
	
	void setup();
	void addLinks(std::vector<PortalLinks> links);
	int getPortalLinkID(int value);
	
	void setBasePortalConnections();
	void setStartEndPortalConnections();
	void addStartPoint();
	void addEndPoint();
	

	PathingInfo generatePaths();
	int getCorrectIndex(int portalIndex, int mapID);

	Map* mpMap;
	GridGraph* mpGridGraph;
	 
	//portalinformation
	PortalGraph* mpPortalGraph;
	PortalPathfinder* mpPortalPathfinder;
	Path* mpPortalPath;
	EndPoint mStartPoint;
	EndPoint mEndPoint;

	std::vector<int> mPathOfMaps;
	int mCurrPathingMap;
	bool mReversed;

	std::map<IDType, Map*> mMapList;
	std::vector<int> mMapIDs;
	std::map<IDType, GridPathfinder*> mPathfinders;
	std::map<IDType, GridGraph*> mGridGraphs;


	PortalInfo mPortals[MAX_PORTALS];

	std::vector<PortalConnection> mBasePortalConnections[MAX_PORTALS];
	std::vector<PortalConnection> mStartEndPortalConnections[MAX_PORTALS + 2];

	std::vector<SpawnInfo> mGrueSpawns;
	std::vector<SpawnInfo> mCandySpawns;
	SpawnInfo mPlayerSpawn;

	
};
