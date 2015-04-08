#include "MapHolder.h"
#include "ChangeMapMessage.h"
#include "GridGraph.h"
#include <Game.h>
#include "GameApp.h"

ChangeMapMessage::ChangeMapMessage(int x, int y, bool leftClick)
: GameMessage(CHANGE_MAP_MESSAGE)
{
	mX = x;
	mY = y;
	mLeftClick = leftClick;
}


void ChangeMapMessage::process()
{
	if (gpMapHolder != NULL)
	{
		gpMapHolder->checkMouseClick(mX, mY, mLeftClick);
	}
}
