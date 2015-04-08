#include "HUD.h"
#include "Vector2D.h"
#include <string>
#include <sstream>
#include <allegro5/allegro_ttf.h>
#include <Map.h>
#include "MapDefines.h"
#include <string>

const int TOP_LEFT_X = 768;
const int X_CENTRE = 896;
const int HELP_Y = 300;
const int HUD_Y = 20;

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
	initHUDText();
}

HUD::~HUD()
{
	al_destroy_font(mpFont);
}

void HUD::draw()
{
	if (mToggleHelp)
		drawHelp();

	for (int i = 0; i < HUD_LINES; ++i)
	{
		std::string fullString = mHUDText[i];
		if (mHUDVals[i] != NULL)
			fullString.append(std::to_string(*mHUDVals[i]));
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, HUD_Y + (i * 20), ALLEGRO_ALIGN_CENTRE, fullString.c_str());
	}

	//display help info
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, TOP_LEFT_X - 20, ALLEGRO_ALIGN_CENTRE, mHelpText[10].c_str());
}

void HUD::drawHelp()
{
	for (int i = 0; i < HELP_LINES - 1; ++i)
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), X_CENTRE, HELP_Y + (i * 40), ALLEGRO_ALIGN_CENTRE, mHelpText[i].c_str());
}

void HUD::initHelpText()
{
	mHelpText[0] = "Left Click: Navigate Portals";
	mHelpText[1] = "Right Click: Path to";
	mHelpText[2] = "ESC: Escape the program";
	mHelpText[3] = "Space: Locate Player";
	mHelpText[4] = "A: Toggle Quick Path";
	mHelpText[5] = "D: Toggle Debug";
	mHelpText[6] = "C: Display Visited Nodes in Debug";
	mHelpText[7] = "G: God Mode";
	mHelpText[8] = "Collect Coins, Avoid Grues, Eat Candy";
	mHelpText[9] = "H/F1: Toggle help";
}

void HUD::initHUDText()
{
	mHUDText[0] = "~~~League of Pacman~~~";
	mHUDText[1] = "Legend of the Coins";
	mHUDText[3] = "Score: ";
	mHUDText[4] = "Grues Eaten: ";
	mHUDText[5] = "Candies: ";
	mHUDText[6] = "Deaths: ";
	mHUDText[7] = "God: ";

	for (int i = 0; i < HUD_LINES; ++i)
		mHUDVals[i] = NULL;
}