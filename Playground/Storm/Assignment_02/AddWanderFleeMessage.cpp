#include "AddWanderFleeMessage.h"
#include "UnitManager.h"
#include "Defines.h"
#include "Game.h"

AddWanderFleeMessage::AddWanderFleeMessage(float distance)
:GameMessage(ADD_WANDER_FLEE)
, mDistance(distance)
{

}

AddWanderFleeMessage::~AddWanderFleeMessage()
{

}

void AddWanderFleeMessage::process()
{
	UNIT_MANAGER->addWanderFlee(mDistance);
}