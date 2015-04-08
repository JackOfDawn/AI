#include "MapHolder.h"
#include "MakeEndPointMessage.h"
#include "GridGraph.h"
#include <Game.h>
#include "GameApp.h"
#include "KinematicUnit.h"
#include "Player.h"
#include "UnitManager.h"

MakeEndPointMessage::MakeEndPointMessage(int x, int y)
: GameMessage(MAKE_END_POINT_MESSAGE)
{
	mX = x;
	mY = y;

}


void MakeEndPointMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Player* pPlayer = dynamic_cast<Player*>(pGame->getUnitManager()->getPlayer());

	if (gpMapHolder != NULL)
	{
		gpMapHolder->generatePlayerEndPoint(mX, mY);
	}
}
