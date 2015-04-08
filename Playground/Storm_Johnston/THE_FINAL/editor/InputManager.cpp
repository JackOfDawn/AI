#include "InputManager.h"
#include "Vector2D.h"
#include "GameMessage.h"
#include "GameMessageManager.h"
#include "SaveMessage.h"
#include "LoadMessage.h"
#include "RemoveBlockMessage.h"
#include "AddBlockMessage.h"
#include "CreateNewMapMessage.h"
#include "ChangeMapMessage.h"
#include "RemoveMapMessage.h"
#include "ToggleHelpMessage.h"
#include "ChangeBlockMessage.h"
#include "Editor.h"
#include "MapDefines.h"
#include "HUD.h"
#include <Game.h>


const int MAP_WIDTH = 768;

InputManager::InputManager()
{
	for (size_t i = 0; i < (int)NUM_KEYS; i++)
	{
		mCurrentKeys[i] = false;
		mReleasedKeys[i] = false;
		mPreviousKeys[i] = false;
	}

	for (size_t i = 0; i < (int)NUM_BUTTONS; i++)
	{
		mCurrentMouseButtons[i] = false;
		mPreviousMouseButtons[i] = false;
	}
}

InputManager::~InputManager()
{
	delete mpKeyboardState;
	mpKeyboardState = NULL;

	delete mpMouseState;
	mpMouseState = NULL;
}

bool InputManager::init()
{
	mpKeyboardState = new ALLEGRO_KEYBOARD_STATE();
	mpMouseState = new ALLEGRO_MOUSE_STATE();
	return true;
}

void InputManager::update()
{
	al_get_mouse_state(mpMouseState);
	al_get_keyboard_state(mpKeyboardState);

	for (size_t i = 0; i < (int)NUM_KEYS; i++)
	{
		mPreviousKeys[i] = mCurrentKeys[i];
	}

	for (size_t i = 0; i < (int)NUM_BUTTONS; i++)
	{
		mPreviousMouseButtons[i] = mCurrentMouseButtons[i];
	}


	//update newKeys
	mCurrentKeys[(int)ESC_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_ESCAPE);
	mCurrentKeys[(int)S_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_S);
	mCurrentKeys[(int)L_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_L);
	mCurrentKeys[(int)ONE_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_1);
	mCurrentKeys[(int)TWO_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_2);
	mCurrentKeys[(int)THREE_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_3);
	mCurrentKeys[(int)FOUR_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_4);
	mCurrentKeys[(int)FIVE_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_5);
	mCurrentKeys[(int)SIX_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_6);
	mCurrentKeys[(int)SEVEN_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_7);
	mCurrentKeys[(int)EIGHT_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_8);
	mCurrentKeys[(int)NINE_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_9);
	mCurrentKeys[(int)ZERO_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_0);
	mCurrentKeys[(int)PLUS_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_EQUALS);
	mCurrentKeys[(int)MINUS_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_MINUS);
	mCurrentKeys[(int)RIGHT_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_RIGHT);
	mCurrentKeys[(int)LEFT_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_LEFT);
	mCurrentKeys[(int)H_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_H);
	mCurrentKeys[(int)F1_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_F1);
	mCurrentKeys[(int)P_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_P);
	mCurrentKeys[(int)G_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_G);
	mCurrentKeys[(int)M_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_M);

	for (size_t i = 0; i < (int)NUM_KEYS; i++)
	{
		mReleasedKeys[i] = (!mCurrentKeys[i] && mPreviousKeys[i]);
	}


	mCurrentMouseButtons[(int)LEFT_CLICK] = al_mouse_button_down(mpMouseState, 1);
	mCurrentMouseButtons[(int)RIGHT_CLICK] = al_mouse_button_down(mpMouseState, 2);

	mCursorLocation.setX(mpMouseState->x);
	mCursorLocation.setY(mpMouseState->y);


	if (mReleasedKeys[(int)ESC_KEY])
	{
		dynamic_cast<Editor*>(gpGame)->markForExit();
	}

	if (mReleasedKeys[(int)S_KEY])
	{
		GameMessage* pMessage = new SaveMessage();
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)L_KEY])
	{
		GameMessage* pMessage = new	LoadMessage();
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)ONE_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(PORTAL_1_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)TWO_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(PORTAL_2_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)THREE_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(PORTAL_3_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)FOUR_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(PORTAL_4_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)FIVE_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(PORTAL_5_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)SIX_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(PORTAL_6_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)SEVEN_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(PORTAL_7_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)EIGHT_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(PORTAL_8_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)NINE_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(PORTAL_9_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)ZERO_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(WALL_BLOCK_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)PLUS_KEY])
	{
		GameMessage* pMessage = new	CreateNewMapMessage();
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}
	
	if (mReleasedKeys[(int)MINUS_KEY])
	{
		GameMessage* pMessage = new	RemoveMapMessage();
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}
	
	if (mReleasedKeys[(int)RIGHT_KEY])
	{
		GameMessage* pMessage = new	ChangeMapMessage(true);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}
	
	if (mReleasedKeys[(int)LEFT_KEY])
	{
		GameMessage* pMessage = new	ChangeMapMessage(false);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}
	
	if (mReleasedKeys[(int)H_KEY] || mReleasedKeys[(int)F1_KEY])
	{
		GameMessage* pMessage = new	ToggleHelpMessage();
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)P_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(PLAYER_SPAWN_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)M_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(MIGHTY_CANDY_ID);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)G_KEY])
	{
		GameMessage* pMessage = new	ChangeBlockMessage(GRUE_SPAWN);
		dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}
	//HandleClicks on the Map
	if (mCursorLocation.getX() < MAP_WIDTH && mCursorLocation.getX() > 0
		&& mCursorLocation.getY() > 0 && mCursorLocation.getY() < MAP_WIDTH	)
	{
		if (mCurrentMouseButtons[(int)LEFT_CLICK])
		{
			GameMessage* pMessage = new	AddBlockMessage(mCursorLocation.getX(), mCursorLocation.getY());
			dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
		}

		if (mCurrentMouseButtons[(int)RIGHT_CLICK])
		{
			GameMessage* pMessage = new	RemoveBlockMessage(mCursorLocation.getX(), mCursorLocation.getY());
			dynamic_cast<Editor*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
		}
	}
	
}