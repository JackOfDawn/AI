#pragma once

#include "allegro5\allegro.h"
#include "Trackable.h"
#include "Vector2D.h"
#include "Defines.h"
#include "GameMessage.h"

enum KeysUsable
{
	ESC_KEY = 0,
	S_KEY,
	L_KEY,
	ONE_KEY,
	TWO_KEY,
	THREE_KEY,
	FOUR_KEY,
	FIVE_KEY,
	SIX_KEY,
	SEVEN_KEY,
	EIGHT_KEY,
	NINE_KEY,
	ZERO_KEY,
	PLUS_KEY,
	MINUS_KEY,
	RIGHT_KEY,
	LEFT_KEY,
	H_KEY,
	F1_KEY,
	NUM_KEYS
};

enum MouseButtons
{
	LEFT_CLICK = 0,
	RIGHT_CLICK,
	NUM_BUTTONS
};

class InputManager : public Trackable
{
public:
	InputManager();
	~InputManager();

	bool init();
	void update();

private:
	ALLEGRO_KEYBOARD_STATE* mpKeyboardState;
	ALLEGRO_MOUSE_STATE* mpMouseState;

	bool mCurrentKeys[(int)NUM_KEYS];
	bool mReleasedKeys[(int)NUM_KEYS];
	bool mPreviousKeys[(int)NUM_KEYS];

	bool mCurrentMouseButtons[(int)NUM_BUTTONS];
	bool mPreviousMouseButtons[(int)NUM_BUTTONS];
	Vector2D mCursorLocation;

};
