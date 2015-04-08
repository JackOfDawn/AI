#pragma once

#include "allegro5\allegro.h"
#include "Trackable.h"
#include "Vector2D.h"
#include "Defines.h"
#include "GameMessageManager.h"

enum KeysUsable
{
	F1_KEY = 0,
	H_KEY, //toggle help
	A_KEY, //toggle holdToPath
	SPACE_KEY, //ShowCharacter
	ENTER, //disables splash

	D_KEY, //Toggle debug
	C_KEY, //show search area'a
	P_KEY, //Show the path
	G_KEY, //God mode

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
	bool mReleasedButtons[(int)NUM_BUTTONS];
	Vector2D mCursorLocation;

	bool debugMode;
	bool holdToPath;

	inline void toggleHoldToPath() { holdToPath = !holdToPath; };
	inline void toggleDebug() { debugMode = !debugMode; };
};