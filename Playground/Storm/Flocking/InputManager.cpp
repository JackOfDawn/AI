#include "InputManager.h"
#include "PlayerMoveToMessage.h"
#include "EndGameMessage.h"
#include "DisplayMouseCoordMessage.h"
#include "AddFlockMessage.h"
#include "DeleteAIMessage.h"
#include "AddWanderFleeMessage.h"
#include "SelectTypeMessage.h"
#include "AdjustWeightMessage.h"
#include "AddAsteroidMessage.h"
#include "RemoveAsteroidMessage.h"
#include "SaveMessage.h"
#include "Game.h"
#include <sstream>

const float ADJUSTMENT_VALUE = .1f;

InputManager::InputManager()
{
	for (int i = 0; i < (int)NUM_KEYS; i++)
	{
		mCurrentKeys[i] = false;
		mPreviousKeys[i] = false;
	}

	for (int i = 0; i < (int)NUM_BUTTONS; i++)
	{
		mCurrentMouseButtons[i] = false;
		mPreviousMouseButtons[i] = false;
	}
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

void InputManager::update(float time)
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

	mCurrentKeys[(int)L_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_L);
	mCurrentKeys[(int)S_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_S);
	mCurrentKeys[(int)D_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_D);
	mCurrentKeys[(int)R_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_R);
	mCurrentKeys[(int)C_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_C);
	mCurrentKeys[(int)G_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_G);
	mCurrentKeys[(int)ESC_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_ESCAPE);
	mCurrentKeys[(int)B_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_B);
	mCurrentKeys[(int)MINUS_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_MINUS);
	mCurrentKeys[(int)EQUALS_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_EQUALS);

	mCurrentKeys[(int)Q_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_Q);
	mCurrentKeys[(int)ONE_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_1);
	mCurrentKeys[(int)TWO_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_2);
	mCurrentKeys[(int)THREE_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_3);
	

	mCurrentMouseButtons[(int)LEFT_CLICK] = al_mouse_button_down(mpMouseState, 1);

	mCursorLocation.setX(mpMouseState->x);
	mCursorLocation.setY(mpMouseState->y);

	mQKeyReleased = (mCurrentKeys[(int)Q_KEY] == false && mPreviousKeys[(int)Q_KEY] == true);
	m1KeyReleased = (mCurrentKeys[(int)ONE_KEY] == false && mPreviousKeys[(int)ONE_KEY] == true);
	m2KeyReleased = (mCurrentKeys[(int)TWO_KEY] == false && mPreviousKeys[(int)TWO_KEY] == true);
	m3KeyReleased = (mCurrentKeys[(int)THREE_KEY] == false && mPreviousKeys[(int)THREE_KEY] == true);
	mLKeyReleased = (mCurrentKeys[(int)L_KEY] == false && mPreviousKeys[(int)L_KEY] == true);
	mSKeyReleased = (mCurrentKeys[(int)S_KEY] == false && mPreviousKeys[(int)S_KEY] == true);
	mDKeyReleased = (mCurrentKeys[(int)D_KEY] == false && mPreviousKeys[(int)D_KEY] == true);
	mBKeyReleased = (mCurrentKeys[(int)B_KEY] == false && mPreviousKeys[(int)B_KEY] == true);
	mRKeyReleased = (mCurrentKeys[(int)R_KEY] == false && mPreviousKeys[(int)R_KEY] == true);
	mCKeyReleased = (mCurrentKeys[(int)C_KEY] == false && mPreviousKeys[(int)C_KEY] == true);
	mGKeyReleased = (mCurrentKeys[(int)G_KEY] == false && mPreviousKeys[(int)G_KEY] == true);
	mMinusKeyReleased = (mCurrentKeys[(int)MINUS_KEY] == false && mPreviousKeys[(int)MINUS_KEY] == true);
	mEqualKeyReleased = (mCurrentKeys[(int)EQUALS_KEY] == false && mPreviousKeys[(int)EQUALS_KEY] == true);
	//Keyboard events


	//asteroids
	if (mQKeyReleased)
	{
		GameMessage* pMessage = new RemoveAsteroidMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	if (m1KeyReleased)
	{
		GameMessage* pMessage = new AddAsteroidMessage(SMALL_ASTEROID_ID);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	if (m2KeyReleased)
	{
		GameMessage* pMessage = new AddAsteroidMessage(MEDIUM_ASTEROID_ID);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	if (m3KeyReleased)
	{
		GameMessage* pMessage = new AddAsteroidMessage(LARGE_ASTEROID_ID);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}


	if (mLKeyReleased)
	{
		GameMessage* pMessage = new SaveMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	if (mGKeyReleased)
	{
		//select group
		GameMessage* pMessage = new SelectTypeMessage(ALIGNMENT);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	if (mCKeyReleased)
	{
		//select cohesion
		GameMessage* pMessage = new SelectTypeMessage(COHESION);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	if (mSKeyReleased)
	{
		//select separation
		GameMessage* pMessage = new SelectTypeMessage(SEPARATION);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	if (mMinusKeyReleased)
	{
		GameMessage* pMessage = new AdjustWeightMessage(ADJUSTMENT_VALUE * -1);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	else if (mEqualKeyReleased)
	{
		GameMessage* pMessage = new AdjustWeightMessage(ADJUSTMENT_VALUE);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}


	if (mBKeyReleased)
	{
		GameMessage* pMessage = new AddFlockMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	if (mRKeyReleased)
	{
		GameMessage* pMessage = new DeleteAIMessage(); //distance is 100
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	if (mCurrentKeys[(int)ESC_KEY])
	{
		GameMessage* pMessage = new EndGameMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	//mouse event

	if (mCurrentMouseButtons[(int)LEFT_CLICK])
	{
		//call player move to
		//GameMessage* pMessage = new PlayerMoveToMessage(mCursorLocation);
		//MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	//GameMessage* pMessage = new DisplayMouseCoordMessage(mCursorLocation);
	//MESSAGE_MANAGER->addMessage(pMessage, 0);

}

