#include "GiveUnitPathMessage.h"
#include "GameApp.h"
#include <Game.h>

GiveUnitPathMessage::GiveUnitPathMessage(std::vector<Vector2D> path, bool reversed)
: GameMessage(GIVE_UNIT_PATH_MESSAGE)
{
	mPath = path;
	mReversed = reversed;
}

void GiveUnitPathMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (pGame != NULL)
	{
		pGame->updateUnitPath(mPath, mReversed);
	}
}