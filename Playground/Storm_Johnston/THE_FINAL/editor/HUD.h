#pragma once
#include "Trackable.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <sstream>
#include <string>
#include <Defines.h>
#include "PortalInformation.h"




const int HELP_LINES = 15;
class Map;
class HUD : public Trackable
{
public:
	HUD(PortalInfo* pPortals, SpawnInfo* playerSpawn);
	~HUD();

	inline void toggleHelp(){ mToggleHelp = !mToggleHelp; };

	void draw();
	void updateMapInfo(Map* pMap, int pos, int total);
	void updateSelectedBlock(IDType blockID);
	

private:
	void drawInfo();
	void drawHelp();
	void initHelpText();
	bool mToggleHelp;

	ALLEGRO_FONT* mpFont;
	std::string mHelpText[HELP_LINES];


	std::string mCurrentMapName;
	int mCurrentMapID;
	int mTotalMaps;
	int mCurrentMap;
	std::string mSelectedBlockType;
	
	//info
	PortalInfo* mpPortals;
	SpawnInfo* mpSpawnInfo;
};