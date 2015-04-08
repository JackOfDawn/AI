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
#include "Grue.h"
#include "UnitManager.h"
#include <fstream>
#include <vector>
#include <allegro5/allegro_ttf.h>

const IDType BACKGROUND_ID = ENDING_SEQUENTIAL_ID + 1;
const IDType UNIT_ID = 1;
const IDType CANDY_ID = 2;
const IDType PUNIT_ID = 3;
const IDType COIN_ID = 4;
const IDType SPLASH_ID = 5;
const int GRID_SQUARE_SIZE = 8;
const std::string gFileName = "MapDictionary.dic";

GameApp::GameApp()
:mpMessageManager(NULL)
,mpDebugDisplay(NULL)
,mpHUD(NULL)
,mScore(0)
,mGruesEaten(0)
,mDeaths(0)
,mCandies(0)
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
	onSplash = true;
	
	//load buffers
	mpGraphicsBufferManager->loadBuffer( BACKGROUND_ID, "wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(UNIT_ID, "grue.png");
	mpGraphicsBufferManager->loadBuffer(PUNIT_ID, "player.png");
	mpGraphicsBufferManager->loadBuffer(COIN_ID, "coin.png");
	mpGraphicsBufferManager->loadBuffer(CANDY_ID, "candy.png");
	mpGraphicsBufferManager->loadBuffer(SPLASH_ID, "DarkCastle.png");

	mpFont = al_load_ttf_font("cour.ttf", 24, 0);
	if (mpFont == NULL)
	{
		printf("ttf font file not loaded properly!\n");
		assert(0);
	}
	
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

	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer(PUNIT_ID);
	if (pPlayerBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(PUNIT_ID, pPlayerBuffer, 0, 0, pPlayerBuffer->getWidth(), pPlayerBuffer->getHeight());
	}

	GraphicsBuffer* pCoinBuffer = mpGraphicsBufferManager->getBuffer(COIN_ID);
	if (pCoinBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(COIN_ID, pCoinBuffer, 0, 0, pCoinBuffer->getWidth(), pCoinBuffer->getHeight());
	}

	GraphicsBuffer* pCandyBuffer = mpGraphicsBufferManager->getBuffer(CANDY_ID);
	if (pCandyBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(CANDY_ID, pCandyBuffer, 0, 0, pCandyBuffer->getWidth(), pCandyBuffer->getHeight());
	}

	GraphicsBuffer* pSplashBuffer = mpGraphicsBufferManager->getBuffer(SPLASH_ID);
	if (pSplashBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(SPLASH_ID, pSplashBuffer, 0, 0, pSplashBuffer->getWidth(), pSplashBuffer->getHeight());
	}

	Sprite* pBackgroundSprite = mpSpriteManager->getSprite(SPLASH_ID);
	pBackgroundSprite->draw(*(mpGraphicsSystem->getBackBuffer()), 0, 0);
	mpGraphicsSystem->swap();


	//create and load the Grid, GridBuffer, and GridRenderer
	gpMapHolder = new MapHolder();
	gpMapHolder->loadMaps(gFileName);

	mpHUD = new HUD();
	mpHUD->hookVal(&mScore, 3);
	mpHUD->hookVal(&mGruesEaten, 4);
	mpHUD->hookVal(&mCandies, 5);
	mpHUD->hookVal(&mDeaths, 6);

	mpInputHandler = new InputManager();
	mpInputHandler->init();

	mpUnitManager = new UnitManager();
	mpUnitManager->init();

	//CreatePlayerUnit;
	Sprite* pSprite = mpSpriteManager->getSprite(PUNIT_ID);
	mpUnitManager->createPlayerUnit(pSprite);

	pSprite = mpSpriteManager->getSprite(UNIT_ID);
	mpUnitManager->createAIUnits(pSprite);

	pSprite = mpSpriteManager->getSprite(COIN_ID);
	mpUnitManager->createCoins(pSprite, 500, 0.5f);

	pSprite = mpSpriteManager->getSprite(CANDY_ID);
	mpUnitManager->createCandies(pSprite);

	mpMasterTimer->start();

	mpHUD->hookVal(mpUnitManager->getPlayer()->getGodRef(), 7);
	debugMode = false;
	return true;
}

void GameApp::cleanup()
{
	al_destroy_font(mpFont);

	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpInputHandler;
	mpInputHandler = NULL;

	delete gpMapHolder;
	gpMapHolder = NULL;

	delete mpUnitManager;
	mpUnitManager = NULL;

	delete mpHUD;
	mpHUD = NULL;
}

void GameApp::beginLoop()
{
	//should be the first thing done
	Game::beginLoop();
}

void GameApp::processLoop()
{

	mpInputHandler->update();

	mpMessageManager->processMessagesForThisframe();

	
	if (onSplash)
	{
		Sprite* pBackgroundSprite = mpSpriteManager->getSprite(SPLASH_ID);
		pBackgroundSprite->draw(*(mpGraphicsSystem->getBackBuffer()), 0, 0);
		string str = "Press Enter";
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), 500, 600, ALLEGRO_ALIGN_CENTRE, str.c_str() );
	}
	else
	{


		//get back buffer
		GraphicsBuffer* pBackBuffer = mpGraphicsSystem->getBackBuffer();
		//copy to back buffer
		getGridVisualizer()->draw(*pBackBuffer);
#ifdef VISUALIZE_PATH
		//show pathfinder visualizer
		//getPathfinder()->drawVisualization(gpMapHolder->getCurrentMap()->getGrid(), pBackBuffer);
		if (debugMode)
			mpUnitManager->drawDebugInfo(pBackBuffer);
#endif


		mpUnitManager->update(LOOP_TARGET_TIME / 1000.0f);
		mpUnitManager->draw(pBackBuffer);

		mpHUD->draw();
		//should be last thing in processLoop
	}
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}


void GameApp::updateUnitPath(std::vector<Vector2D> path, bool reversed)
{
	//mpUnit->dynamicSeek(path, reversed);
}



Grid* GameApp::getGrid()
{
	return gpMapHolder->getCurrentMap()->getGrid();
}

GridPathfinder* GameApp::getPathfinder()
{
	return gpMapHolder->getCurrentGridPathfinder();
}

GridGraph* GameApp::getGridGraph()
{
	return gpMapHolder->getCurrentGridGraph();
}

GridVisualizer* GameApp::getGridVisualizer()
{
	return gpMapHolder->getCurrentMap()->getGridVisualizer();
}

