#ifndef INPUTMANAGER
#define INPUTMANAGER

#include "Trackable.h"
#include <allegro5/allegro.h>
class InputManager : public Trackable
{
public:
	InputManager();
	~InputManager();

	bool init();
	void update(float time);

private:
	bool mAPressed;
	bool mSPressed;
	bool mDPressed;
};

#endif