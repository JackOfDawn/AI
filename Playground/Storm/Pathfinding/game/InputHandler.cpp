#include "InputHandler.h"
#include "Vector2D.h"
#include "PathToMessage.h"
#include "GameMessage.h"
#include "ChangeAlgorithmMessage.h"
#include "DrawVisitedMessage.h"

#include "GameApp.h"

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

	mCurrentKeys[(int)F_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_F);
	mCurrentKeys[(int)D_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_D);
	mCurrentKeys[(int)A_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_A);
	mCurrentKeys[(int)C_KEY] = al_key_down(mpKeyboardState, ALLEGRO_KEY_C);

	mCurrentMouseButtons[(int)LEFT_CLICK] = al_mouse_button_down(mpMouseState, 1);
	mCurrentMouseButtons[(int)RIGHT_CLICK] = al_mouse_button_down(mpMouseState, 2);

	mCursorLocation.setX(mpMouseState->x);
	mCursorLocation.setY(mpMouseState->y);

	mFKeyReleased = (mCurrentKeys[(int)F_KEY] == false && mPreviousKeys[(int)F_KEY] == true);
	mDKeyReleased = (mCurrentKeys[(int)D_KEY] == false && mPreviousKeys[(int)D_KEY] == true);
	mAKeyReleased = (mCurrentKeys[(int)A_KEY] == false && mPreviousKeys[(int)A_KEY] == true);
	mCKeyReleased = (mCurrentKeys[(int)C_KEY] == false && mPreviousKeys[(int)C_KEY] == true);




	if (mFKeyReleased)
	{
		GameMessage* pMessage = new ChangeAlgorithmMessage(3);
		dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}
	if (mDKeyReleased)
	{
		GameMessage* pMessage = new ChangeAlgorithmMessage(1);
		dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}
	if (mAKeyReleased)
	{
		GameMessage* pMessage = new ChangeAlgorithmMessage(2);
		dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	if (mCKeyReleased)
	{
		GameMessage* pMessage = new DrawVisitedMessage();
		dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
	}

	static Vector2D lastLeftPos(0.0f, 0.0f);
	static Vector2D lastRightPos(0.0f, 0.0f);

	if (mCurrentMouseButtons[(int)LEFT_CLICK])
	{
		Vector2D pos(mpMouseState->x, mpMouseState->y);
		if (lastLeftPos.getX() != pos.getX() || lastLeftPos.getY() != lastLeftPos.getY())
		{
			GameMessage* pMessage = new PathToMessage(pos, lastRightPos);
			dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage,0);
			lastLeftPos = pos;
		}
	}
	if (mCurrentMouseButtons[(int)RIGHT_CLICK])
	{
		Vector2D pos(mpMouseState->x, mpMouseState->y);
		if (lastRightPos.getX() != pos.getX() || lastRightPos.getY() != pos.getY())
		{
			GameMessage* pMessage = new PathToMessage(lastLeftPos, pos);
			dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage,0);
			lastRightPos = pos;
		}
	}

}