#pragma once

#include "allegro5\allegro.h"
#include "Trackable.h"
#include "Vector2D.h"
#include "Defines.h"
#include "GameMessageManager.h"

enum KeysUsable
{
	F_KEY = 0,
	L_KEY,
	B_KEY,
	S_KEY,
	D_KEY,
	R_KEY,
	C_KEY,
	G_KEY,
	ESC_KEY,
	Q_KEY,
	ONE_KEY,
	TWO_KEY,
	THREE_KEY,
	MINUS_KEY,
	EQUALS_KEY,
	NUM_KEYS
};
enum MouseButtons
{
	LEFT_CLICK = 0,
	NUM_BUTTONS
};

class InputManager : public Trackable
{
public:
	InputManager();
	~InputManager();

	bool init();
	void update(float time);

private:
	ALLEGRO_KEYBOARD_STATE* mpKeyboardState;
	ALLEGRO_MOUSE_STATE* mpMouseState;

	bool mCurrentKeys[(int)NUM_KEYS];
	bool mPreviousKeys[(int)NUM_KEYS];

	bool mCurrentMouseButtons[(int)NUM_BUTTONS];
	bool mPreviousMouseButtons[(int)NUM_BUTTONS];
	Vector2D mCursorLocation;

	bool mFKeyReleased;
	bool mLKeyReleased;
	bool mSKeyReleased;
	bool m1KeyReleased;
	bool m2KeyReleased;
	bool m3KeyReleased;
	bool mQKeyReleased;
	bool mDKeyReleased;
	bool mBKeyReleased;
	bool mRKeyReleased;
	bool mCKeyReleased;
	bool mGKeyReleased;
	bool mMinusKeyReleased;
	bool mEqualKeyReleased;

};