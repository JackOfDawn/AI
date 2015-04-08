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
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	MapHolder* pMapHolder = pGame->getMapHolder();
	if (pGame != NULL)
	{
		pMapHolder->checkMouseClick(mX, mY, mLeftClick);
	}
}
