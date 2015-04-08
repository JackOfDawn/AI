#include "AddWanderSeekMessage.h"
#include "UnitManager.h"
#include "Defines.h"
#include "Game.h"

AddWanderSeekMessage::AddWanderSeekMessage(float distance)
:GameMessage(ADD_WANDER_SEEK)
, mDistance(distance)
{
	
}

AddWanderSeekMessage::~AddWanderSeekMessage()
{

}

void AddWanderSeekMessage::process()
{
	UNIT_MANAGER->addWanderSeek(mDistance);
}