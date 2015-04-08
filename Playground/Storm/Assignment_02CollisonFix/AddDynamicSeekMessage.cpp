#include "AddDynamicSeekMessage.h"
#include "UnitManager.h"
#include "Defines.h"
#include "Game.h"


AddDynamicSeekMessage::AddDynamicSeekMessage(float distance)
:GameMessage(ADD_DYNAMIC_SEEK_MESSAGE)
, mDistance(distance)
{

}
AddDynamicSeekMessage::~AddDynamicSeekMessage()
{

}

void AddDynamicSeekMessage::process()
{
	UNIT_MANAGER->addDynamicSeek(mDistance);
}