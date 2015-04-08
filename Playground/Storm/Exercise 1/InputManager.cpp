#include "InputManager.h"
#include "PlayerMoveToMessage.h"
#include "EndGameMessage.h"
#include "DisplayMouseCoordMessage.h"
#include "AddDynamicArriveMessage.h"
#include "AddDynamicSeekMessage.h"
#include "DeleteAIMessage.h"
#include "Game.h"
#include <sstream>

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

	mCurrentKeys[(int)A_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_A);
	mCurrentKeys[(int)S_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_S);
	mCurrentKeys[(int)D_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_D);
	mCurrentKeys[(int)ESC_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_ESCAPE);

	mCurrentMouseButtons[(int)LEFT_CLICK] = al_mouse_button_down(mpMouseState, 1);

	mCursorLocation.setX(mpMouseState->x);
	mCursorLocation.setY(mpMouseState->y);

	mAKeyReleased = (mCurrentKeys[(int)A_KEY] == false && mPreviousKeys[(int)A_KEY] == true);
	mSKeyReleased = (mCurrentKeys[(int)S_KEY] == false && mPreviousKeys[(int)S_KEY] == true);
	mDKeyReleased = (mCurrentKeys[(int)D_KEY] == false && mPreviousKeys[(int)D_KEY] == true);

	//Keyboard events
	if (mAKeyReleased)
	{
		GameMessage* pMessage = new AddDynamicArriveMessage(200); //distance is 200
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	if (mSKeyReleased)
	{
		GameMessage* pMessage = new AddDynamicSeekMessage(100); //distance is 100
		MESSAGE_MANAGER->addMessage(pMessage, 0);

	}

	if (mDKeyReleased)
	{
		//Call the event for D
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
		GameMessage* pMessage = new PlayerMoveToMessage(mCursorLocation);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	GameMessage* pMessage = new DisplayMouseCoordMessage(mCursorLocation);
	MESSAGE_MANAGER->addMessage(pMessage, 0);

}

