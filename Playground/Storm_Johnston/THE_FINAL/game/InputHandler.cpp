#include "InputHandler.h"
#include "Vector2D.h"
#include "PathToMessage.h"
#include "GameMessage.h"
#include "ChangeAlgorithmMessage.h"
#include "DrawVisitedMessage.h"
#include "ToggleHelpMessage.h"
#include "ToggleDebugMessage.h"
#include "ChangeMapMessage.h"
#include "MakeEndPointMessage.h"
#include "GodModeMessage.h"
#include "JumpToPlayerMap.h"
#include "GameApp.h"


const int MAP_WIDTH = 768;

InputManager::InputManager()
{
	for (int i = 0; i < (int)NUM_KEYS; i++)
	{
		mCurrentKeys[i] = false;
		mReleasedKeys[i] = false;
		mPreviousKeys[i] = false;
	}

	for (int i = 0; i < (int)NUM_BUTTONS; i++)
	{
		mCurrentMouseButtons[i] = false;
		mPreviousMouseButtons[i] = false;
	}

	debugMode = false;
	holdToPath = false;

}

InputManager::~InputManager()
{
	delete mpKeyboardState;
	delete mpMouseState;

	al_uninstall_keyboard();
	al_uninstall_mouse();
}

bool InputManager::init()
{
	if (!al_install_keyboard())
	{
		std::cout << "This could be a problem... keyboard didn't initialize properly.\n";
		return false;
	}

	if (!al_install_mouse())
	{
		std::cout << "This could be a problem... mouse didn't initialize properly.\n";
		return false;
	}

	mpKeyboardState = new ALLEGRO_KEYBOARD_STATE();
	mpMouseState = new ALLEGRO_MOUSE_STATE();

	return true;
}

void InputManager::update()
{
	al_get_mouse_state(mpMouseState);
	al_get_keyboard_state(mpKeyboardState);


	for (int i = 0; i < (int)NUM_KEYS; i++)
	{
		mPreviousKeys[i] = mCurrentKeys[i];
	}
	for (int i = 0; i < (int)NUM_BUTTONS; i++)
	{
		mPreviousMouseButtons[i] = mCurrentMouseButtons[i];
	}

	mCurrentKeys[(int)ENTER] = al_key_down(mpKeyboardState, ALLEGRO_KEY_ENTER);
	mCurrentKeys[(int)F1_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_F1);
	mCurrentKeys[(int)H_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_H);
	mCurrentKeys[(int)A_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_A);
	mCurrentKeys[(int)C_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_C);
	mCurrentKeys[(int)D_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_D);
	mCurrentKeys[(int)P_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_P);
	mCurrentKeys[(int)G_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_G);
	mCurrentKeys[(int)SPACE_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_SPACE);

	mCurrentMouseButtons[(int)LEFT_CLICK] = al_mouse_button_down(mpMouseState, 1);
	mCurrentMouseButtons[(int)RIGHT_CLICK] = al_mouse_button_down(mpMouseState, 2);

	mCursorLocation.setX(mpMouseState->x);
	mCursorLocation.setY(mpMouseState->y);

	for (int i = 0; i < (int)NUM_KEYS; i++)
	{
		mReleasedKeys[i] = !mCurrentKeys[i] && mPreviousKeys[i];
	}

	for (int i = 0; i < (int)NUM_BUTTONS; i++) //mouse shits
	{
		mReleasedButtons[i] = !mCurrentMouseButtons[i] && mPreviousMouseButtons[i];
	}



	//EVENTS
	if ((mReleasedKeys[(int)H_KEY]) || (mReleasedKeys[(int)F1_KEY]))
	{
		GameMessage* pMessage = new ToggleHelpMessage();
		dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if ((mReleasedKeys[(int)D_KEY]))
	{
		toggleDebug();
		GameMessage* pMessage = new ToggleDebugMessage();
		dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if ((mReleasedKeys[(int)SPACE_KEY]))
	{
		GameMessage* pMessage = new JumpToPlayerMapMessage();
		dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mReleasedKeys[(int)ENTER])
	{
		dynamic_cast<GameApp*>(gpGame)->startGame();
	}
	if ((mReleasedKeys[(int)G_KEY]))
	{
		GameMessage* pMessage = new GodModeMessage();
		dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if ((mReleasedKeys[(int)A_KEY]))
	{
		toggleHoldToPath();
	}

	if (debugMode)
	{
		if ((mReleasedKeys[(int)C_KEY]))
		{
			GameMessage* pMessage = new DrawVisitedMessage();
			dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
		}

		if ((mReleasedKeys[(int)P_KEY]))
		{
			
		}
	}


	static Vector2D lastLeftPos(0.0f, 0.0f);
	static Vector2D lastRightPos(0.0f, 0.0f);



	if (mCursorLocation.getX() < MAP_WIDTH && mCursorLocation.getX() > 0
		&& mCursorLocation.getY() > 0 && mCursorLocation.getY() < MAP_WIDTH)
	{
		if (mReleasedButtons[(int)LEFT_CLICK])//||	
		{
			GameMessage* pMessage = new ChangeMapMessage(mpMouseState->x, mpMouseState->y, true);
			dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
		}


		if (holdToPath)
		{
			if (mCurrentMouseButtons[(int)RIGHT_CLICK])// && mPreviousMouseButtons[(int)RIGHT_CLICK])
			{
				GameMessage* pMessage = new MakeEndPointMessage(mpMouseState->x, mpMouseState->y);
				dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
			}
		}
		else
		{
			if (!mCurrentMouseButtons[(int)RIGHT_CLICK] && mPreviousMouseButtons[(int)RIGHT_CLICK])
			{
				GameMessage* pMessage = new MakeEndPointMessage(mpMouseState->x, mpMouseState->y);
				dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
			}
		}
	}
}