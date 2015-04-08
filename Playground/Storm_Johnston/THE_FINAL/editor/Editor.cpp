#include <fstream>
#include <allegro5/allegro_primitives.h>

#include "Game.h"
#include "Editor.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "Grid.h"
#include "GridVisualizer.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Vector2D.h"
#include <Map.h>
#include "GameMessageManager.h"
#include "InputManager.h"
#include "HUD.h"
#include <sstream>


using namespace std;

const int HD_GRID_SQUARE_SIZE = 12;
const int SD_GRID_SQUARE_SIZE = 24;
const IDType BACKGROUND_ID = ENDING_SEQUENTIAL_ID + 1;

Editor::Editor()
:Game()
, mpMap(NULL)
, mpInputManager(NULL)
, mpMessageManager(NULL)
, mpHUD(NULL)
{
	mMapList = vector<Map*>();
	mCounterMap = 0;
	NEXT_ID = 1;
}

Editor::~Editor()
{
	cleanup();
}

bool Editor::init()
{
	bool retVal = Game::init();
	if( retVal == false )
	{
		return false;
	}

	mpMap = new Map();
	mpMap->initNewMap(mpGraphicsSystem->getHeight(), mpGraphicsSystem->getHeight(), HD_GRID_SQUARE_SIZE, NEXT_ID);
	NEXT_ID++;
	mMapList.push_back(mpMap);
	//load buffers
	mpGraphicsBufferManager->loadBuffer( BACKGROUND_ID, "wallpaper.bmp");

	//reateNewMap();
	mPlayerSpawn = SpawnInfo();

	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( BACKGROUND_ID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, pBackGroundBuffer->getWidth(), pBackGroundBuffer->getHeight() );
	}
	
	mpMessageManager = new GameMessageManager();
	mpInputManager = new InputManager();
	mpInputManager->init();
	mpHUD = new HUD(mPortals, &mPlayerSpawn);
	mpHUD->updateMapInfo(mpMap, mCounterMap + 1, mMapList.size());


	resetPortals();

	setBlockType(WALL_BLOCK_ID);
	mpMasterTimer->start();
	return true;
}

void Editor::cleanup()
{

	cleanupMaps();
	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpInputManager;
	mpInputManager = NULL;

	delete mpHUD;
	mpHUD = NULL;
}

void Editor::cleanupMaps()
{
	for each (Map* map in mMapList)
	{
		delete map;
		mpMap = NULL;
	}
	mMapList.clear();
	NEXT_ID = 1;
}
void Editor::beginLoop()
{
	//should be the first thing done
	Game::beginLoop();
}

void Editor::processLoop()
{

	//copy to back buffer
	mpMap->getGridVisualizer()->draw(*(mpGraphicsSystem->getBackBuffer()));


	//Update input
	mpInputManager->update();

	//process messages
	mpMessageManager->processMessagesForThisframe();


	//should be last thing in processLoop
	mpHUD->draw();
	Game::processLoop();
}

bool Editor::endLoop()
{
	return Game::endLoop();
}

void Editor::changeMap(bool forward)
{
	if (forward)//move to the next map in the list
	{
		mCounterMap = (mCounterMap + 1) % mMapList.size();
	}
	else //move back a map
	{
		
		if (mCounterMap == 0)
		{
			mCounterMap = mMapList.size() - 1;
		}
		else
		{
			mCounterMap--;
		}
	}

	fetchCorrectMap();
}
void Editor::fetchCorrectMap()
{
	mpMap = mMapList.at(mCounterMap);
	mpHUD->updateMapInfo(mpMap, mCounterMap + 1, mMapList.size());
}

void Editor::createNewMap()
{
	mpMap = new Map();
	mpMap->initNewMap(mpGraphicsSystem->getHeight(), mpGraphicsSystem->getHeight(), SD_GRID_SQUARE_SIZE, NEXT_ID);
	NEXT_ID++;
	mMapList.push_back(mpMap);
	mCounterMap = mMapList.size() - 1;
	fetchCorrectMap();
	printf("Created new map with ID: %i \n", NEXT_ID - 1);
}

void Editor::removeCurrentMap()
{
	if (mpMap->getID() != 1)
	{
		mMapList.erase(mMapList.begin() + mCounterMap); //removes map from list
		delete mpMap; //deletes it
		changeMap(false);
		printf("Map deleted \n");
	}
	else //it is the default map, can't delete it
	{
		printf("Can't delete the main map\n");
	}
	//adjust to the new map
}

void Editor::removePlayerSpawn(int x, int y)
{
	int value = mpMap->getGrid()->getValueAtPixelXY(x, y);
	if (value == PLAYER_SPAWN_ID)
	{
		mpMap->setPlayer(INVALID_ID);
		mPlayerSpawn = SpawnInfo();
	}
}

void Editor::removePortal(int x, int y)
{
	int value = mpMap->getGrid()->getValueAtPixelXY(x, y);

	if (value >= MIN_PORTAL_ID)
	{
		int iter = value % 100;
		if (mPortals[iter].map1ID == mpMap->getID())
		{
			mPortals[iter].map1ID = INVALID_ID;
			mPortals[iter].portalIndexMap1 = INVALID_ID;
		}
		else if (mPortals[iter].map2ID == mpMap->getID())
		{
			mPortals[iter].map2ID = INVALID_ID;
			mPortals[iter].portalIndexMap2 = INVALID_ID;
		}
		
	}
}

void Editor::removeBlock(int x, int y)
{
	//check if it is a portal, if so update the portal list
	//mpMap->getGrid()->getValueAtPixelXY(x, y);
	removePortal(x, y);
	removePlayerSpawn(x, y);

	if (mpMap->getGrid()->getValueAtPixelXY(x, y) == GRUE_SPAWN_ID)
	{
		mpMap->removeGrue(x, y);
	}

	if (mpMap->getGrid()->getValueAtPixelXY(x, y) == CANDY_SPAWN_ID)
	{
		mpMap->removeCandy(x, y);
	}

	mpMap->getGrid()->setValueAtPixelXY(x, y, CLEAR_VALUE);
	mpMap->getGridVisualizer()->setModified();
}

void Editor::addBlock(int x, int y)
{
	removePortal(x, y);
	removePlayerSpawn(x, y);
	//if a portal, check if it is avaliable to place
	if (mSelectedBlock >= MIN_PORTAL_ID)
	{
		bool addable = checkandAddPortal(mSelectedBlock,mpMap->getGrid()->getSquareIndexFromPixelXY(x, y));
		if (!addable)
			return;
	}
	
	//if it is a grue spawn add it to the map's list
	if (mSelectedBlock == GRUE_SPAWN_ID)
	{
		mpMap->addGrue(x, y);
	}
	//if it is a candy spawn add it to the map's list
	if (mSelectedBlock == CANDY_SPAWN_ID)
	{
		mpMap->addCandy(x, y);
	}


	//if Player spawn doesn't exists add it.
	if (mSelectedBlock == PLAYER_SPAWN_ID)
	{
		if (mPlayerSpawn.mapID != INVALID_ID)
		{
			return;
		}
		else //we can add it
		{
			mPlayerSpawn.location = mpMap->getGrid()->getSquareIndexFromPixelXY(x, y);
			mPlayerSpawn.mapID = mpMap->getID();
			mpMap->setPlayer(mPlayerSpawn.location);
		}
	}
	
	mpMap->getGrid()->setValueAtPixelXY(x, y, mSelectedBlock); //switch this value to the chosen value
	mpMap->getGridVisualizer()->setModified();
}

bool Editor::checkandAddPortal(int value, int index)
{
	if (value >= MIN_PORTAL_ID)
	{
		int iter = value % MIN_PORTAL_ID;
		if (mPortals[iter].map1ID != INVALID_ID && mPortals[iter].map2ID != INVALID_ID)
		{
			//portals are full
			return false;
		}
		else if (mPortals[iter].map1ID == INVALID_ID)
		{
			mPortals[iter].map1ID = mpMap->getID();
			mPortals[iter].portalIndexMap1 = index;
		}
		else if (mPortals[iter].map2ID == INVALID_ID)
		{
			mPortals[iter].map2ID = mpMap->getID();
			mPortals[iter].portalIndexMap2 = index;
		}
	}

	return true;
}

void Editor::setBlockType(int blockType)
{
	mSelectedBlock = blockType;
	mpHUD->updateSelectedBlock(blockType);
}

void Editor::save()
{
	ofstream theStream(FILE_NAME);
	this->saveGrid(theStream);
	theStream.close();
	cout << "Grid saved!\n";
}

void Editor::load()
{
	ifstream theStream(FILE_NAME);
	this->loadGrid(theStream);
	theStream.close();
	this->getCurrentMap()->getGridVisualizer()->setModified();
	cout << "Grid loaded!\n";
}


void Editor::saveGrid( ofstream& theStream)
{
	theStream << mMapList.size() << endl; //add the total number of maps to the file

	//save each of the maps
	for (size_t i = 0; i < mMapList.size(); i++)
	{
		mMapList[i]->setLinks(mPortals);
		mMapList[i]->saveGrid(theStream);
	}
}

void Editor::loadGrid( std::ifstream& theStream )
{
	resetPortals();
	stringstream ss = stringstream();
	int numMaps;
	theStream >> numMaps;
	std::getline(theStream, ss.str()); //finish off the line
	//ss >> numMaps; // get number of maps form the dictionary file

	//clean up old maps;
	this->cleanupMaps();

	
	for (int i = 0; i < numMaps; i++)
	{
		mMapList.push_back(new Map()); //create a new base map
		mMapList[i]->loadGrid(theStream); //load data into the map
		
		if (mMapList[i]->getID() > NEXT_ID)
			NEXT_ID = mMapList[i]->getID();
		printf("Loaded a map \n");

		addLinks(mMapList[i]->getLinks()); 
		if (mMapList[i]->getPlayer() != INVALID_ID)
		{
			mPlayerSpawn.location = mMapList[i]->getPlayer();
			mPlayerSpawn.mapID = mMapList[i]->getID();
		}
	}

	NEXT_ID++;
	mpMap = mMapList[0];
	mCounterMap = 0;
	fetchCorrectMap();
}

void Editor::addLinks(std::vector<PortalLinks> links)
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

void Editor::resetPortals()
{
	for (size_t i = 0; i < MAX_PORTALS; i++)
	{
		mPortals[i] = PortalInfo();
		mPortals[i].portalID = MIN_PORTAL_ID + i;
	}
}