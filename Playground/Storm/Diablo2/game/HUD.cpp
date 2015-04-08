#include "HUD.h"
#include "Vector2D.h"
#include <string>
#include <sstream>
#include <allegro5/allegro_ttf.h>
#include <Map.h>
#include "MapDefines.h"

const int TOP_LEFT_X = 768;
const int X_CENTRE = 896;

HUD::HUD()
{
	mpFont = al_load_ttf_font("cour.ttf", 12, 0);
	if (mpFont == NULL)
	{
		printf("ttf font file not loaded properly!\n");
		assert(0);
	}
	mToggleHelp = false;
	
	initHelpText();
}

HUD::~HUD()
{
	al_destroy_font(mpFont);
}


void HUD::draw()
{
	if (mToggleHelp)
		drawHelp();

	//display help info
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, TOP_LEFT_X - 20, ALLEGRO_ALIGN_CENTRE, mHelpText[6].c_str());
}

void HUD::drawHelp()
{
	for (int i = 0; i < HELP_LINES - 1; ++i)
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, 20 + (i * 40), ALLEGRO_ALIGN_CENTRE, mHelpText[i].c_str());
}

void HUD::initHelpText()
{
	mHelpText[0] = "Left Click: Place start pos";
	mHelpText[1] = "Right Click: Place end pos";
	mHelpText[2] = "ESC: Escape the program";
	mHelpText[3] = "Click on a portal to switch Maps";
	mHelpText[4] = "Maps will switch when AI hits portal";
	mHelpText[5] = "Enjoy";
	mHelpText[6] = "H/F1: Toggle help";

}