#pragma once
#include "Trackable.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <sstream>
#include <string>
#include <Defines.h>

const int HELP_LINES = 10;
const int HUD_LINES = 8;

class HUD : public Trackable
{
public:
	HUD();
	~HUD();

	inline void toggleHelp(){ mToggleHelp = !mToggleHelp; };
	void hookVal(int* val, int index){ mHUDVals[index] = val; }

	void draw();

private:
	void drawHelp();
	void initHelpText();
	void initHUDText();
	bool mToggleHelp;

	ALLEGRO_FONT* mpFont;
	std::string mHelpText[HELP_LINES];
	std::string mHUDText[HUD_LINES];
	int* mHUDVals[HUD_LINES];
};