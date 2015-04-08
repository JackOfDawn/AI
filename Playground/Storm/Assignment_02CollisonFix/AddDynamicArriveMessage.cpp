#include "AddDynamicArriveMessage.h"
#include "Defines.h"
#include "Game.h"
#include "GameMessageManager.h"
#include "UnitManager.h"

AddDynamicArriveMessage::AddDynamicArriveMessage(float distance)
:GameMessage(ADD_DYNAMIC_ARRIVE_MESSAGE)
, mDistance(distance)
{
}

AddDynamicArriveMessage::~AddDynamicArriveMessage()
{
}
void AddDynamicArriveMessage::process()
{
	UNIT_MANAGER->addDynamicArrive(mDistance);
}