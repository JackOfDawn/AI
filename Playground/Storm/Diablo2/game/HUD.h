#pragma once
#include "Trackable.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <sstream>
#include <string>
#include <Defines.h>

const int HELP_LINES = 7;

class HUD : public Trackable
{
public:
	HUD();
	~HUD();

	inline void toggleHelp(){ mToggleHelp = !mToggleHelp; };

	void draw();

private:

	void drawHelp();
	void initHelpText();
	bool mToggleHelp;

	ALLEGRO_FONT* mpFont;
	std::string mHelpText[HELP_LINES];


};