#include "DeleteAIMessage.h"
#include "Game.h"
#include "UnitManager.h"
#include "Defines.h"

DeleteAIMessage::DeleteAIMessage()
:GameMessage(DELETE_AI_MESSAGE)
{
}
DeleteAIMessage::~DeleteAIMessage()
{
}

void DeleteAIMessage::process()
{
	UNIT_MANAGER->deleteAIUnit();
}