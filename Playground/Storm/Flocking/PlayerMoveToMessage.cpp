#include "Game.h"
#include "KinematicUnit.h"
#include "UnitManager.h"
#include "GameMessageManager.h"
#include "PlayerMoveToMessage.h"

PlayerMoveToMessage::PlayerMoveToMessage( const Vector2D& pos )
:GameMessage(PLAYER_MOVETO_MESSAGE)
,mPos(pos)
{
}

PlayerMoveToMessage::~PlayerMoveToMessage()
{
}

void PlayerMoveToMessage::process()
{
	//gpGame->getUnitManager()->
	UNIT_MANAGER->getPlayer()->arrive(mPos);
	//gpGame->getPlayerUnit()->arrive(mPos);
}

