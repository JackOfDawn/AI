#include <stdio.h>
#include <assert.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <sstream>
#include <math.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "GameMessageManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Timer.h"
#include "KinematicUnit.h"
#include "PlayerMoveToMessage.h"
#include "UnitManager.h"
#include "InputManager.h"
#include "Defines.h"

Game* gpGame = NULL;

const int WALL_SIZE = 10;

Game::Game()
:mpGraphicsSystem(NULL)
, mpGraphicsBufferManager(NULL)
, mpSpriteManager(NULL)
, mpLoopTimer(NULL)
, mpMasterTimer(NULL)
, mShouldExit(false)
, mpFont(NULL)
, mpSample(NULL)
, mpUnitManager(NULL)
, mBackgroundBufferID(INVALID_ID) 
	//,mSmurfBufferID(INVALID_ID)
{
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	//startup allegro
	if(!al_init()) 
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return false;
	}

	//create and init GraphicsSystem
	mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mpGraphicsSystem->init( WIDTH, HEIGHT );
	if(!goodGraphics) 
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager();
	mpSpriteManager = new SpriteManager();

	//inputManager
	mpInputManager = new InputManager();
	mpInputManager->init();

	//startup a lot of allegro stuff

	//load image loader addon
	if( !al_init_image_addon() )
	{
		fprintf(stderr, "image addon failed to load!\n");
		return false;
	}

	//install audio stuff
	if( !al_install_audio() )
	{
		fprintf(stderr, "failed to initialize sound!\n");
		return false;
	}

	if(!al_init_acodec_addon())
	{
		fprintf(stderr, "failed to initialize audio codecs!\n");
		return false;
	}
 
	if (!al_reserve_samples(1))
	{
		fprintf(stderr, "failed to reserve samples!\n");
		return false;
	}

	//should be somewhere else!
	al_init_font_addon();
	if( !al_init_ttf_addon() )
	{
		printf( "ttf font addon not initted properly!\n" ); 
		return false;
	}

	//actually load the font
	mpFont = al_load_ttf_font( "cour.ttf", 20, 0 );
	if( mpFont == NULL )
	{
		printf( "ttf font file not loaded properly!\n" ); 
		return false;
	}

	//show the mouse
	if( !al_hide_mouse_cursor( mpGraphicsSystem->getDisplay() ) )
	{
		printf( "Mouse cursor not able to be hidden!\n" ); 
		return false;
	}

	if( !al_init_primitives_addon() )
	{
		printf( "Primitives addon not added!\n" ); 
		return false;
	}

	//load the sample
	mpSample = al_load_sample( "clapping.wav" );
	if (!mpSample)
	{
		printf( "Audio clip sample not loaded!\n" ); 
		return false;
	}

	mpMessageManager = new GameMessageManager();

	mpUnitManager = new UnitManager();

	//load buffers
	mBackgroundBufferID = mpGraphicsBufferManager->loadBuffer("space.png");
	mPlayerIconBufferID = mpGraphicsBufferManager->loadBuffer("target.png");
	mEnemyIconBufferID = mpGraphicsBufferManager->loadBuffer("enemy.png");
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, pBackGroundBuffer->getWidth(), pBackGroundBuffer->getHeight() );
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	Sprite* pArrowSprite = NULL;
	if( pPlayerBuffer != NULL )
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite( PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, pPlayerBuffer->getWidth(), pPlayerBuffer->getHeight() );
	}
	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer( mEnemyIconBufferID );
	Sprite* pEnemyArrow = NULL;
	if( pAIBuffer != NULL )
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite( AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, pAIBuffer->getWidth(), pAIBuffer->getHeight() );
	}



	//setup units
	Vector2D pos( 200.0f, 400.0f );
	Vector2D vel( 0.0f, 0.0f );
	//mpUnit = new KinematicUnit( pArrowSprite, pos, 1, vel, 0.0f, 200.0f, 10.0f );
	mpUnitManager->createPlayerUnit(pArrowSprite, pos, 1, vel, 0.0f, 200.0f, 10.0f, NULL_STEERING);
	Vector2D pos2( 1000.0f, 500.0f );
	Vector2D vel2( 0.0f, 0.0f );


	Vector2D pos3( 500.0f, 500.0f );
	//mpAIUnit2 = new KinematicUnit( pEnemyArrow, pos3, 1, vel2, 0.0f, 180.0f, 100.0f );
	//give steering behavior
	//mpAIUnit2->dynamicSeek( mpUnit );  
	mpUnitManager->createAIUnit(pEnemyArrow, pos3, 1, vel2, 0.0f, 180.0f, 100.0f, DYNAMIC_WANDER_FLEE);

	

	//buildBoundingWalls
	mWalls = vector<Rect2D>();
	mWalls.push_back(Rect2D(0, 0, WIDTH, WALL_SIZE)); //top wall
	mWalls.push_back(Rect2D(0, 0, WALL_SIZE, HEIGHT)); //Left wall
	mWalls.push_back(Rect2D(WIDTH - WALL_SIZE, 0, WALL_SIZE, HEIGHT)); //right wall
	mWalls.push_back(Rect2D(0, HEIGHT - WALL_SIZE, WIDTH, WALL_SIZE)); //bottom wall

	return true;
}

void Game::cleanup()
{
	//delete units
	delete mpUnitManager;
	mpUnitManager = NULL;
	//delete mpUnit;
	//mpUnit = NULL;


	delete mpInputManager;
	mpInputManager = NULL;
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpMessageManager;
	mpMessageManager = NULL;

	al_destroy_sample(mpSample);
	mpSample = NULL;
	al_destroy_font(mpFont);
	mpFont = NULL;

	//shutdown components
	al_uninstall_audio();
	al_shutdown_image_addon();
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_shutdown_primitives_addon();

}

void Game::beginLoop()
{
	mpLoopTimer->start();
}
	
void Game::processLoop()
{
	//Check for Potential Collisions 

	//update units
	mpUnitManager->update(LOOP_TARGET_TIME / 1000.0f);
	
	//draw background
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite( BACKGROUND_SPRITE_ID );
	pBackgroundSprite->draw( *(mpGraphicsSystem->getBackBuffer()), 0, 0 );
	//pBackgroundSprite->drawRect(*(mpGraphicsSystem->getBackBuffer()), 0, 0, 10, HEIGHT);

	

	//draw units
	mpUnitManager->draw(GRAPHICS_SYSTEM->getBackBuffer());
	drawMouseLocation();

	//drawWalls


	//process messages
	mpMessageManager->processMessagesForThisframe();
	
	//update input
	mpInputManager->update(LOOP_TARGET_TIME);

	
	mpGraphicsSystem->swap();
}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );
	return mShouldExit;
}

void Game::drawMouseLocation()
{
	stringstream mousePos;
	mousePos << (int)mMousePos.getX() << ":" << (int)mMousePos.getY();

	//write text at mouse position
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), (int)mMousePos.getX(), (int)mMousePos.getY(), ALLEGRO_ALIGN_CENTRE, mousePos.str().c_str());
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}

Vector2D genLocation(Vector2D &centralPos, float distance)
{
	float x, y, angle;
	Vector2D newPos = Vector2D();
	//get the angle
	x = genRandomBinomial();
	y = genRandomBinomial();

	angle = atan2f(y, x);

	//get new x position
	newPos.setX((distance * cos(angle)) + centralPos.getX());
	//get new y position
	newPos.setY((distance * sin(angle)) + centralPos.getY());
	
	//return the newvector
	return newPos;
}


/*#include "System.h"
#include "Game.h"
#include "GameMessageManager.h"
#include "PlayerMoveToMessage.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Sprite.h"
#include "KinematicUnit.h"
#include "Vector2D.h"
*/
/*Game* gpGame = NULL;

Game::Game()
:mpMessageManager(NULL)
,mFrameNum(0)
,mMarkedForExit(false)
{
}

Game::~Game()
{
	delete mpArrowBuffer;
	delete mpArrowSprite;
	delete mpEnemyArrowBuffer;
	delete mpEnemyArrowSprite;
	delete mpUnit;
	delete mpAIUnit;
	delete mpAIUnit2;
	delete mpMessageManager;
}

void Game::init()
{
	mpMessageManager = new GameMessageManager();

	mpArrowBuffer = new GraphicsBuffer( "arrow.bmp" );
	mpArrowSprite = new Sprite( mpArrowBuffer, 0, 0, 32, 32 );
	mpEnemyArrowBuffer = new GraphicsBuffer( "enemy-arrow.bmp" );
	mpEnemyArrowSprite = new Sprite( mpEnemyArrowBuffer, 0, 0, 32, 32 );
	
	Vector2D pos( 0.0f, 0.0f );
	Vector2D vel( 0.0f, 0.0f );
	mpUnit = new KinematicUnit( mpArrowSprite, pos, 1, vel, 0.0f, 200.0f, 10.0f );

	Vector2D pos2( 1000.0f, 500.0f );
	Vector2D vel2( 0.0f, 0.0f );
	mpAIUnit = new KinematicUnit( mpEnemyArrowSprite, pos2, 1, vel2, 0.0f, 180.0f, 100.0f );
	mpAIUnit->dynamicArrive( mpUnit ); 
	//mpAIUnit->arrive( mpUnit->getPosition() ); 

	Vector2D pos3( 500.0f, 500.0f );
	mpAIUnit2 = new KinematicUnit( mpEnemyArrowSprite, pos3, 1, vel2, 0.0f, 180.0f, 100.0f );
	mpAIUnit2->dynamicSeek( mpUnit );  
	//mpAIUnit2->seek( mpUnit->getPosition() );  
}

bool Game::update()
{
	mpUnit->update( LOOP_TARGET_TIME/1000.0f );
	mpUnit->draw( GRAPHICS_SYSTEM->getBackBuffer() );
	mpAIUnit->update( LOOP_TARGET_TIME/1000.0f );
	mpAIUnit->draw( GRAPHICS_SYSTEM->getBackBuffer() );
	mpAIUnit2->update( LOOP_TARGET_TIME/1000.0f );
	mpAIUnit2->draw( GRAPHICS_SYSTEM->getBackBuffer() );

	mpMessageManager->processMessagesForThisframe();

	//get input - should be moved someplace better
	if( mouse_b & 1 )//left mouse click
	{
		Vector2D pos( mouse_x, mouse_y );
		GameMessage* pMessage = new PlayerMoveToMessage( pos );
		MESSAGE_MANAGER->addMessage( pMessage, 0 );
	}

	mFrameNum++;
	return mMarkedForExit;
}*/