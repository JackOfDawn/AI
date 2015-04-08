#include "CheckEndPathMessage.h"
#include "GameApp.h"
#include "MapHolder.h"
#include <Game.h>


CheckPathEndMessage::CheckPathEndMessage()
: GameMessage(CHECK_PATH_END_MESSAGE)
{

}

void CheckPathEndMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	MapHolder* pMapHolder = pGame->getMapHolder();
	if (pGame != NULL)
	{
		pMapHolder->checkPathMaps();
	}
}