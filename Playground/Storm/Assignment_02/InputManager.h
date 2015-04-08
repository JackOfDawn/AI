#pragma once

#include "allegro5\allegro.h"
#include "Trackable.h"
#include "Vector2D.h"
#include "Defines.h"
#include "GameMessageManager.h"

enum KeysUsable
{
	F_KEY = 0,
	S_KEY,
	D_KEY,
	ESC_KEY,
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
	bool mSKeyReleased;
	bool mDKeyReleased;

};