#pragma once
#include <Defines.h>
//Block IDs
typedef int BlockID;

const int EMPTY_BLOCK_ID = 0;
const int WALL_BLOCK_ID = 1;
const int PLAYER_SPAWN_ID = 2;
const int GRUE_SPAWN_ID = 3;
const int CANDY_SPAWN_ID = 4;
const int PORTAL_1_ID = 100;
const int PORTAL_2_ID = 101;
const int PORTAL_3_ID = 102;
const int PORTAL_4_ID = 103;
const int PORTAL_5_ID = 104;
const int PORTAL_6_ID = 105;
const int PORTAL_7_ID = 106;
const int PORTAL_8_ID = 107;
const int PORTAL_9_ID = 108;

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

