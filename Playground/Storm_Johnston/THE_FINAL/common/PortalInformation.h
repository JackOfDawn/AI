#pragma once
struct PortalInfo
{
	PortalInfo()
	{
		portalID = -1;
		portalIndexMap1 = -1;
		portalIndexMap2 = -1;
		map1ID = -1;
		map2ID = -1;
	}

	int portalID;
	int portalIndexMap1;
	int portalIndexMap2;
	int map1ID;
	int map2ID;
	
};

struct PortalLinks
{
	int portalID;
	int portalIndex;
	int map2ID;
};

struct SpawnInfo
{
	SpawnInfo()
	{
		mapID = -1;
		location = -1;
	}

	int location;
	int mapID;
};
