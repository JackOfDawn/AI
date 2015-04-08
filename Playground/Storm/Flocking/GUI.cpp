#include "GUI.h"
#include "GraphicsBuffer.h"
#include <allegro5/allegro_font.h>
#include "Flocking.h"
#include <iomanip>

GUI::GUI(ALLEGRO_FONT* pFont)
{
	mpFont = pFont;
	mAlignString << "ALIGNMENT: " << FlockingSteering::getWeight(ALIGNMENT);
	mSeparString << "SEPARATION: " << FlockingSteering::getWeight(SEPARATION);
	mCohesString << "COHESION: " << FlockingSteering::getWeight(COHESION);
	mUnselected = al_map_rgb(255, 255, 255);
	mSelected = al_map_rgb(255, 0, 0);
	mSelectedType = SEPARATION;
}


void GUI::update()
{
	mAlignString.str("");
	mSeparString.str("");
	mCohesString.str("");
	mAlignString << "ALIGNMENT: " << std::fixed << std::setprecision(1) << FlockingSteering::getWeight(ALIGNMENT);
	mSeparString << "SEPARATION: " << std::fixed << std::setprecision(1) << FlockingSteering::getWeight(SEPARATION);
	mCohesString << "COHESION: " << std::fixed << std::setprecision(1) << FlockingSteering::getWeight(COHESION);
}

void GUI::draw(GraphicsBuffer* pBuffer)
{
	switch (mSelectedType)
	{
	case ALIGNMENT:
		al_draw_text(mpFont, mSelected, xPOS, yPOS, ALLEGRO_ALIGN_LEFT, mAlignString.str().c_str());
		al_draw_text(mpFont, mUnselected, xPOS, yPOS * 2, ALLEGRO_ALIGN_LEFT, mSeparString.str().c_str());
		al_draw_text(mpFont, mUnselected, xPOS, yPOS * 3, ALLEGRO_ALIGN_LEFT, mCohesString.str().c_str());
		break;
	case COHESION:
		al_draw_text(mpFont, mUnselected, xPOS, yPOS, ALLEGRO_ALIGN_LEFT, mAlignString.str().c_str());
		al_draw_text(mpFont, mUnselected, xPOS, yPOS * 2, ALLEGRO_ALIGN_LEFT, mSeparString.str().c_str());
		al_draw_text(mpFont, mSelected, xPOS, yPOS * 3, ALLEGRO_ALIGN_LEFT, mCohesString.str().c_str());
		break;
	case SEPARATION:
		al_draw_text(mpFont, mUnselected, xPOS, yPOS, ALLEGRO_ALIGN_LEFT, mAlignString.str().c_str());
		al_draw_text(mpFont, mSelected, xPOS, yPOS * 2, ALLEGRO_ALIGN_LEFT, mSeparString.str().c_str());
		al_draw_text(mpFont, mUnselected, xPOS, yPOS * 3, ALLEGRO_ALIGN_LEFT, mCohesString.str().c_str());
	}
}

void GUI::selectType(WeightType type)
{
	mSelectedType = type;
}

void GUI::adjustType(float adj)
{
	FlockingSteering::adjustWeight(mSelectedType, adj);
	update();
}
