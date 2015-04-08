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

class Map;
class GridGraph;
class GridPathfinder;

const int START_ID = MAX_PORTALS + MIN_PORTAL_ID;
const int END_ID = MAX_PORTALS + MIN_PORTAL_ID + 1;

struct EndPoint
{
	EndPoint()
	{
		index = INVALID_ID;
		mapID = INVALID_ID;
		portalID = INVALID_ID;
	}
	bool isValid()
	{
		return (index != INVALID_ID && mapID != INVALID_ID && portalID != INVALID_ID);
	}
	int index;
	int mapID;
	int portalID;
};
class MapHolder : public Trackable
{
public:

	MapHolder();
	~MapHolder();

	inline Map* getCurrentMap() { return mpMap; };
	inline GridGraph* getCurrentGridGraph() { return mpGridGraph; };
	inline GridPathfinder* getCurrentGridPathfinder() { return mpPathfinder; }

	void loadMaps(std::string fileName);
	void clear();
	void checkMouseClick(int x, int y, bool leftClick);
	void changeMaps(IDType mapID);
	bool isValidPathMap();


	void giveTheVectorPath();
	void updateMapFromUnit();

	void checkPathMaps();

private:
	bool setPathPoint(int x, int y, bool leftClick);
	void setup();
	void addLinks(std::vector<PortalLinks> links);
	int getPortalLinkID(int value);
	void changeMapToID(int mapID);
	void setBasePortalConnections();
	void setStartEndPortalConnections();
	void addStartPoint();
	void addEndPoint();
	

	void generatePaths();
	int getCorrectIndex(int portalIndex, int mapID);

	Map* mpMap;
	GridGraph* mpGridGraph;
	GridPathfinder* mpPathfinder;
	
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
	std::map<IDType, GridPathfinder*> mPathfinders;
	std::map<IDType, GridGraph*> mGridGraphs;


	PortalInfo mPortals[MAX_PORTALS];

	std::vector<PortalConnection> mBasePortalConnections[MAX_PORTALS];
	std::vector<PortalConnection> mStartEndPortalConnections[MAX_PORTALS + 2];
};
