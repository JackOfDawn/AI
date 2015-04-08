#include "InputManager.h"
#include "AddDynamicArriveMessage.h"
#include "AddDynamicSeekMessage.h"
#include "DeleteUnitMessage.h"
#include "GameMessageManager.h"
#include "PlayerMoveToMessage.h"
#include "Game.h"
#include "ExitMessage.h"
#include <sstream>
#include "AddBoidMessage.h"
#include "RemoveBoidMessage.h"

InputManager::InputManager()
{
	mAPressed = false;
	mSPressed = false;
	mDPressed = false;
}

InputManager::~InputManager()
{
	// uninstall keyboard and mouse
	al_uninstall_keyboard();
	al_uninstall_mouse();
}

bool InputManager::init()
{
	// install keyboard
	if( !al_install_keyboard() )
	{
		printf( "Keyboard not installed!\n" ); 
		return false;
	}

	// install mouse
	if( !al_install_mouse() )
	{
		printf( "Mouse not installed!\n" ); 
		return false;
	}

	
}

void InputManager::update(float time)
{
	//get mouse state
	ALLEGRO_MOUSE_STATE mouseState;
	al_get_mouse_state( &mouseState );

	if( al_mouse_button_down( &mouseState, 1 ) )//left mouse click
	{
		Vector2D pos( mouseState.x, mouseState.y );
		GameMessage* pMessage = new PlayerMoveToMessage( pos );
		MESSAGE_MANAGER->addMessage( pMessage, 0 );
	}

	//create mouse text
	stringstream mousePos;
	mousePos << mouseState.x << ":" << mouseState.y;

	//write text at mouse position
	al_draw_text( gpGame->getFont(), al_map_rgb( 255, 255, 255 ), mouseState.x, mouseState.y, ALLEGRO_ALIGN_CENTRE, mousePos.str().c_str() );

	//get current keyboard state
	ALLEGRO_KEYBOARD_STATE keyState;
	al_get_keyboard_state( &keyState );

	//if escape key was down then exit the loop
	if( al_key_down( &keyState, ALLEGRO_KEY_ESCAPE ))
	{
		ExitMessage* message = new ExitMessage();
		gpGame->getMessageManager()->addMessage(message, 0);
	}

	// add a new dynamic arrive unit if the A key was pressed
	if (al_key_down(&keyState, ALLEGRO_KEY_A) && !mAPressed)
	{
		// add boid
		AddBoidMessage* message = new AddBoidMessage();
		gpGame->getMessageManager()->addMessage(message, 0);

		mAPressed = true;
	}
	else if (!al_key_down(&keyState, ALLEGRO_KEY_A))
	{
		mAPressed = false;
	}

	

	// delete a unit if the D key was pressed
	if (al_key_down(&keyState, ALLEGRO_KEY_D) && !mDPressed)
	{
		// remove a boid
		mDPressed = true;
	}
	else if (!al_key_down(&keyState, ALLEGRO_KEY_D))
	{
		mDPressed = false;
	}
}