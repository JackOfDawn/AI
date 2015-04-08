#include "JumpToPlayerMap.h"

#include "MapHolder.h"
#include "SetEndPointMessage.h"
#include "GridGraph.h"
#include <Game.h>
#include "GameApp.h"
#include "KinematicUnit.h"
#include "Player.h"
#include "UnitManager.h"

JumpToPlayerMapMessage::JumpToPlayerMapMessage()
: GameMessage(CHANGE_MAP_MESSAGE)
{
}


void JumpToPlayerMapMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Player* pPlayer = dynamic_cast<Player*>(pGame->getUnitManager()->getPlayer());

	if (pPlayer != NULL && gpMapHolder != NULL)
	{
		gpMapHolder->changeMapToID(pPlayer->getMapID());
	}
}
