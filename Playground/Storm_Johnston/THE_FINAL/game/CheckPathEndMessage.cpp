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

	if (gpMapHolder != NULL)
	{
		gpMapHolder->checkPathMaps();
	}
}