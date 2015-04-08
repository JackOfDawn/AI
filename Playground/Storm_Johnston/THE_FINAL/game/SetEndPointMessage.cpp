#include "MapHolder.h"
#include "SetEndPointMessage.h"
#include "GridGraph.h"
#include <Game.h>
#include "GameApp.h"
#include "KinematicUnit.h"
#include "Player.h"
#include "UnitManager.h"

SetEndPointMessage::SetEndPointMessage(EndPoint endPoint)
: GameMessage(MAKE_END_POINT_MESSAGE)
{
	mEndPoint = endPoint;

}


void SetEndPointMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Player* pPlayer = dynamic_cast<Player*>(pGame->getUnitManager()->getPlayer());

	if (pPlayer != NULL)
	{
		pPlayer->generatePathing(mEndPoint);
	}
}
