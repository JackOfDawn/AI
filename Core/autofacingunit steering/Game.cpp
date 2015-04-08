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
#include <random>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "GameMessageManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Timer.h"
#include "KinematicUnit.h"
#include "AutoFacingUnit.h"
#include "PlayerMoveToMessage.h"
#include "DebugDisplay.h"
#include "AutoFacingUnitDebugContent.h"

Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;

Game::Game()
	:mpGraphicsSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mpDebugDisplay(NULL)
	,mShouldExit(false)
	,mpFont(NULL)
	,mpSample(NULL)
	,mBackgroundBufferID(INVALID_ID)
	,mpInputManager(NULL)
	//,mSmurfBufferID(INVALID_ID)
{
	srand(time(NULL));
}

Game::~Game()
{
	cleanup();
}

int Game::GetWidth()
{
	return WIDTH;
}

int Game::GetHeight()
{
	return HEIGHT;
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

	//should probably be done in the InputSystem!
	if( !al_install_keyboard() )
	{
		printf( "Keyboard not installed!\n" ); 
		return false;
	}

	//should probably be done in the InputSystem!
	if( !al_install_mouse() )
	{
		printf( "Mouse not installed!\n" ); 
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

	mpInputManager = new InputManager();

	mpInputManager->init();

	//show the mouse
	/*if( !al_hide_mouse_cursor( mpGraphicsSystem->getDisplay() ) )
	{
		printf( "Mouse cursor not able to be hidden!\n" ); 
		return false;
	}*/

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

	//load buffers
	mBackgroundBufferID = mpGraphicsBufferManager->loadBuffer("wallpaper.bmp");
	mPlayerIconBufferID = mpGraphicsBufferManager->loadBuffer("arrow.bmp");
	mEnemyIconBufferID = mpGraphicsBufferManager->loadBuffer("enemy-arrow.bmp");
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, pBackGroundBuffer->getWidth(), pBackGroundBuffer->getHeight() );
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	pArrowSprite = NULL;
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
mpUnitManager = new UnitManager();
	mpUnitManager->init();
	//setup units
//	Vector2D pos( 0.0f, 0.0f );
//	Vector2D vel( 0.0f, 0.0f );
//	AutoFacingUnit* mpUnit = new AutoFacingUnit( pArrowSprite, pos, 1, vel, 0.0f, 200.0f, 100.0f );
//	mpUnitManager->addUnit(mpUnit);
//	Vector2D pos2( 1000.0f, 500.0f );
//	Vector2D vel2( 0.0f, 0.0f );
//	AutoFacingUnit* mpAIUnit = new AutoFacingUnit( pEnemyArrow, pos2, 1, vel2, 0.0f, 180.0f, 100.0f );
//	//give steering behavior
//	mpAIUnit->dynamicArrive( mpUnit ); 
//mpUnitManager->addUnit(mpAIUnit);
//	Vector2D pos3( 500.0f, 500.0f );
//	AutoFacingUnit* mpAIUnit2 = new AutoFacingUnit( pEnemyArrow, pos3, 1, vel2, 0.0f, 180.0f, 100.0f );
//	//give steering behavior
//	mpAIUnit2->dynamicSeek( mpUnit );  
//	mpUnitManager->addUnit(mpAIUnit2);
	// add units to the unit manager
	

	
	
	

	//debug display
	//AutoFacingUnitDebugContent* pContent = new AutoFacingUnitDebugContent( dynamic_cast<AutoFacingUnit*>(mpUnitManager->getUnit(1)) );
	//mpDebugDisplay = new DebugDisplay( Vector2D(0,12), pContent );
	return true;
}

void Game::cleanup()
{


	delete mpDebugDisplay;
	mpDebugDisplay = NULL;

	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpInputManager;
	mpInputManager = NULL;

	delete mpUnitManager;
	mpUnitManager = NULL;

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
	//update units

	mpInputManager->update(LOOP_TARGET_TIME / 1000.0f);

	mpUnitManager->update( LOOP_TARGET_TIME / 1000.0f);
	
	//draw background
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite( BACKGROUND_SPRITE_ID );
	pBackgroundSprite->draw( *(mpGraphicsSystem->getBackBuffer()), 0, 0 );

	//draw units

	mpUnitManager->draw(GRAPHICS_SYSTEM->getBackBuffer());

	mpMessageManager->processMessagesForThisframe();

	//get input - should be moved someplace better
	ALLEGRO_MOUSE_STATE mouseState;
	al_get_mouse_state( &mouseState );

	if( al_mouse_button_down( &mouseState, 1 ) )//left mouse click
	{
		Vector2D pos( mouseState.x, mouseState.y );
		GameMessage* pMessage = new PlayerMoveToMessage( pos );
		MESSAGE_MANAGER->addMessage( pMessage, 0 );
	}

	//mpDebugDisplay->draw( GRAPHICS_SYSTEM->getBackBuffer() );

	//all this should be moved to InputManager!!!
	{
		//get mouse state
		//ALLEGRO_MOUSE_STATE mouseState;
		//al_get_mouse_state( &mouseState );

		//create mouse text
		//stringstream mousePos;
		//mousePos << mouseState.x << ":" << mouseState.y;

		//write text at mouse position
		//al_draw_text( mpFont, al_map_rgb( 255, 255, 255 ), mouseState.x, mouseState.y, ALLEGRO_ALIGN_CENTRE, mousePos.str().c_str() );

		mpGraphicsSystem->swap();

		//get current keyboard state
		ALLEGRO_KEYBOARD_STATE keyState;
		al_get_keyboard_state( &keyState );

		//if escape key was down then exit the loop
		if( al_key_down( &keyState, ALLEGRO_KEY_ESCAPE ) )
		{
			mShouldExit = true;
		}
	}
}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );
	return mShouldExit;
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

float mapRotationToRange( float rotation, float low, float high )
{
	while( rotation < low )
	{
		rotation += ( 2.0f * PI );
	}

	while( rotation > high )
	{
		rotation -= ( 2.0f * PI );
	}
	return rotation;
}

void Game::setShouldExit(bool exit)
{
	mShouldExit = exit;
}

Sprite* Game::getArrowSprite()
{
	return pArrowSprite;
}