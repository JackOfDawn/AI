#include "HUD.h"
#include "Vector2D.h"
#include <string>
#include <sstream>
#include <allegro5/allegro_ttf.h>
#include <Map.h>


const int TOP_LEFT_X = 768;
const int X_CENTRE = 896;

HUD::HUD(PortalInfo* pPortals, SpawnInfo* playerSpawn )
{
	mpFont = al_load_ttf_font("cour.ttf", 12, 0);
	if (mpFont == NULL)
	{
		printf("ttf font file not loaded properly!\n");
		assert(0);
	}
	mToggleHelp = false;
	
	mCurrentMapName = "INVALID";
	mCurrentMapID = -1;
	mTotalMaps = -1;
	mCurrentMap = -1;
	mSelectedBlockType = "INVALID";
	mpPortals = pPortals;
	mpSpawnInfo = playerSpawn;
	initHelpText();
}

HUD::~HUD()
{
	al_destroy_font(mpFont);
}

void HUD::updateMapInfo(Map* pMap, int pos, int total)
{
	mCurrentMapName = pMap->getFileName();
	mCurrentMapID = pMap->getID();
	mTotalMaps = total;
	mCurrentMap = pos;

}

void HUD::updateSelectedBlock(IDType blockType)
{
	switch (blockType)
	{
	case WALL_BLOCK_ID:
		mSelectedBlockType = "WALL";
		break;
	case CANDY_SPAWN_ID:
		mSelectedBlockType = "MIGHTY CANDY SPAWN";
		break;
	case GRUE_SPAWN_ID:
		mSelectedBlockType = "GRUE SPAWN";
		break;
	case PLAYER_SPAWN_ID:
		mSelectedBlockType = "PLAYER SPAWN";
		break;
	case PORTAL_1_ID:
		mSelectedBlockType = "PORTAL 1";
		break;
	case PORTAL_2_ID:
		mSelectedBlockType = "PORTAL 2";
		break;
	case PORTAL_3_ID:
		mSelectedBlockType = "PORTAL 3";
		break;
	case PORTAL_4_ID:
		mSelectedBlockType = "PORTAL 4";
		break;
	case PORTAL_5_ID:
		mSelectedBlockType = "PORTAL 5";
		break;
	case PORTAL_6_ID:
		mSelectedBlockType = "PORTAL 6";
		break;
	case PORTAL_7_ID:
		mSelectedBlockType = "PORTAL 7";
		break;
	case PORTAL_8_ID:
		mSelectedBlockType = "PORTAL 8";
		break;
	case PORTAL_9_ID:
		mSelectedBlockType = "PORTAL 9";
		break;
	}
}


void HUD::draw()
{
	if (mToggleHelp)
	{
		drawHelp();
	}
	else
	{
		drawInfo();
	}
	//display help info
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, TOP_LEFT_X - 20, ALLEGRO_ALIGN_CENTRE, mHelpText[14].c_str());
}

void HUD::drawHelp()
{
	for (int i = 0; i < HELP_LINES - 1; ++i)
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, 20 + (i * 40), ALLEGRO_ALIGN_CENTRE, mHelpText[i].c_str());
}

void HUD::drawInfo()
{
	int yPos = 20;
	int adjustment = 40;
	std::stringstream ss;
	

	//display basics
	ss << "File Name: " << mCurrentMapName;
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, yPos, ALLEGRO_ALIGN_CENTRE, ss.str().c_str());

	yPos += adjustment;
	ss.clear();
	ss.str("");
	ss << "ID: " << mCurrentMapID;
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, yPos, ALLEGRO_ALIGN_CENTRE, ss.str().c_str());

	yPos += adjustment;
	ss.clear();
	ss.str("");
	ss << mCurrentMap << " / " << mTotalMaps << " Maps";
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, yPos, ALLEGRO_ALIGN_CENTRE, ss.str().c_str());

	yPos += adjustment;
	ss.clear();
	ss.str("");
	ss << "Block Type: " << mSelectedBlockType;
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, yPos, ALLEGRO_ALIGN_CENTRE, ss.str().c_str());



	//display Player Spawn Info
	yPos += adjustment;
	ss.clear();
	ss.str("");
	ss << "Player Spawn MapID: ";
	if (mpSpawnInfo->mapID == INVALID_ID)
	{
		ss << "INVALID";
	}
	else
	{
		ss << mpSpawnInfo->mapID;
	}
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, yPos, ALLEGRO_ALIGN_CENTRE, ss.str().c_str());




	yPos += adjustment;
	ss.clear();
	ss.str("");
	ss << "PORTALS";
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, yPos, ALLEGRO_ALIGN_CENTRE, ss.str().c_str());
	yPos += adjustment /2;
	ss.clear();
	ss.str("");
	ss << "MAP ID | PORTAL ID | MAP ID";
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, yPos, ALLEGRO_ALIGN_CENTRE, ss.str().c_str());


	//display portal info
	for (int i = 0; i < MAX_PORTALS; ++i)
	{
		yPos += adjustment;
		ss.clear();
		ss.str("");

		if (mpPortals[i].map1ID == -1)
			ss << "INVALID";
		else
			ss << mpPortals[i].map1ID;

		ss << " | " << (mpPortals[i].portalID % MIN_PORTAL_ID + 1) << " | ";

		if (mpPortals[i].map2ID == -1)
			ss << "INVALID";
		else
			ss << mpPortals[i].map2ID;

		al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, yPos, ALLEGRO_ALIGN_CENTRE, ss.str().c_str());
	}

	

	
}

void HUD::initHelpText()
{
	mHelpText[0] = "Left Click: Add a block";
	mHelpText[1] = "Right Click: Remove a block";
	mHelpText[2] = "ESC: Escape the Editor";
	mHelpText[3] = "S: Save all the maps";
	mHelpText[4] = "L: Load saved maps";
	mHelpText[5] = "1: Select a wall block";
	mHelpText[6] = "2-9: Select portal blocks";
	mHelpText[7] = "+: Create a new 32x32 map";
	mHelpText[8] = "-: Remove current 32x32 map";
	mHelpText[9] = "Right Arrow: Cycle to the right";
	mHelpText[10] = "Left Arrow: Cycle to the left";
	mHelpText[11] = "M: Select CANDY spawn";
	mHelpText[12] = "G: Select GRUE spawn";
	mHelpText[13] = "P: Select PLAYER spawn";
	mHelpText[14] = "H/F1: Toggle help";

}