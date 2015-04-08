#pragma once
#include "Trackable.h"
#include <allegro5/allegro_font.h>
#include <allegro5\allegro_color.h>
#include <sstream>
#include "Defines.h"
class GraphicsBuffer;

const float xPOS = 50;
const float yPOS = 25;
const float yOFFSET = 25;

class GUI : public Trackable
{
public:
	GUI(ALLEGRO_FONT* pFont);
	~GUI(){};

	void update();
	void draw(GraphicsBuffer* pBuffer);
	void selectType(WeightType type);
	void adjustType(float adj);

private:
	WeightType mSelectedType;
	ALLEGRO_FONT* mpFont;
	ALLEGRO_COLOR mSelected;
	ALLEGRO_COLOR mUnselected;

	std::stringstream mAlignString;
	std::stringstream mSeparString;
	std::stringstream mCohesString;
};
