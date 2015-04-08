#include <allegro5/allegro.h>
#include "Game.h"
#include "GameApp.h"
#include "GameMessageManager.h"
#include "PathToMessage.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Vector2D.h"
#include "Grid.h"
#include "GridGraph.h"
#include "Connection.h"
#include "Path.h"
#include "DepthFirstPathfinder.h"
#include "DijkstraPathfinder.h"
#include "AStarPathFinder.h"
#include "Pathfinder.h"
#include "GridPathfinder.h"
#include "GridVisualizer.h"
#include "DebugDisplay.h"
#include "PathfindingDebugContent.h"
#include "InputHandler.h"
#include "MapHolder.h"
#include <Map.h>
#include "KinematicUnit.h"
#include "HUD.h"

#include <fstream>
#include <vector>

const IDType BACKGROUND_ID = ENDING_SEQUENTIAL_ID + 1;
const IDType UNIT_ID = 1;
const int GRID_SQUARE_SIZE = 8;
const std::string gFileName = "MapDictionary.dic";

GameApp::GameApp()
:mpMessageManager(NULL)
,mpDebugDisplay(NULL)
, mpMapHolder(NULL)
, mpHUD(NULL)
{
}

GameApp::~GameApp()
{
	cleanup();
}

bool GameApp::init()
{
	bool retVal = Game::init();
	if( retVal == false )
	{

		return false;
	}

	

	mpMessageManager = new GameMessageManager();

	//create and load the Grid, GridBuffer, and GridRenderer
	mpMapHolder = new MapHolder();
	mpMapHolder->loadMaps(gFileName);

	//load buffers
	mpGraphicsBufferManager->loadBuffer( BACKGROUND_ID, "wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(UNIT_ID, "unit.png");

	mpHUD = new HUD;
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( BACKGROUND_ID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, pBackGroundBuffer->getWidth(), pBackGroundBuffer->getHeight() );
	}

	GraphicsBuffer* pUnitBuffer = mpGraphicsBufferManager->getBuffer(UNIT_ID);
	if (pBackGroundBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(UNIT_ID, pUnitBuffer, 0, 0, pUnitBuffer->getWidth(), pUnitBuffer->getHeight());
	}

	mpInputHandler = new InputManager();
	mpInputHandler->init();

	Sprite* pSprite = mpSpriteManager->getSprite(UNIT_ID);
	mpUnit = new KinematicUnit(pSprite, Vector2D(200, 400), 0, Vector2D(1, 1), 0, 20);
	std::vector<Vector2D> vectors = vector<Vector2D>();
	vectors.push_back(Vector2D(0, 0));
	vectors.push_back(Vector2D(1000, 1000));
	vectors.push_back(Vector2D(0, 1000));
	mpUnit->dynamicSeek(vectors, true);

	//debug display
	PathfindingDebugContent* pContent = new PathfindingDebugContent( getPathfinder() );
	mpDebugDisplay = new DebugDisplay( Vector2D(0,12), pContent );

	mpMasterTimer->start();
	return true;
}

void GameApp::cleanup()
{
	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpInputHandler;
	mpInputHandler = NULL;

	delete mpMapHolder;
	mpMapHolder = NULL;

	delete mpDebugDisplay;
	mpDebugDisplay = NULL;

	delete mpHUD;
	mpHUD = NULL;

	delete mpUnit;
	mpUnit = NULL;
}

void GameApp::beginLoop()
{
	//should be the first thing done
	Game::beginLoop();
}

void GameApp::processLoop()
{
	
	//get back buffer
	GraphicsBuffer* pBackBuffer = mpGraphicsSystem->getBackBuffer();
	//copy to back buffer
	getGridVisualizer()->draw( *pBackBuffer );
#ifdef VISUALIZE_PATH
	//show pathfinder visualizer
	getPathfinder()->drawVisualization(mpMapHolder->getCurrentMap()->getGrid(), pBackBuffer);
#endif

	

	delete mpDebugDisplay;
	PathfindingDebugContent* pContent = new PathfindingDebugContent(getPathfinder());
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);

	mpDebugDisplay->draw( pBackBuffer );
	mpInputHandler->update();

	mpMessageManager->processMessagesForThisframe();


	mpUnit->update(LOOP_TARGET_TIME / 1000.0f);
	if (mpMapHolder->isValidPathMap())
	{
		mpUnit->draw(pBackBuffer);
	}

	mpHUD->draw();
	//should be last thing in processLoop
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}


void GameApp::updateUnitPath(std::vector<Vector2D> path, bool reversed)
{
	mpUnit->dynamicSeek(path, reversed);
}



Grid* GameApp::getGrid()
{
	return mpMapHolder->getCurrentMap()->getGrid();
}

GridPathfinder* GameApp::getPathfinder()
{
	return mpMapHolder->getCurrentGridPathfinder();
}

GridGraph* GameApp::getGridGraph()
{
	return mpMapHolder->getCurrentGridGraph();
}

GridVisualizer* GameApp::getGridVisualizer()
{
	return mpMapHolder->getCurrentMap()->getGridVisualizer();
}

