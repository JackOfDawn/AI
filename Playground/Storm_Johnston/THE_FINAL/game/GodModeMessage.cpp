#include "GodModeMessage.h"

#include "MapHolder.h"
#include "SetEndPointMessage.h"
#include "GridGraph.h"
#include <Game.h>
#include "GameApp.h"
#include "KinematicUnit.h"
#include "Player.h"
#include "UnitManager.h"

GodModeMessage::GodModeMessage()
: GameMessage(GOD_MODE_MESSAGE)
{
}


void GodModeMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Player* pPlayer = dynamic_cast<Player*>(pGame->getUnitManager()->getPlayer());

	pPlayer->godMode();
}
