#include "RemoveAsteroidMessage.h"
#include "Game.h"
#include "UnitManager.h"
#include "Defines.h"

RemoveAsteroidMessage::RemoveAsteroidMessage()
:GameMessage(REMOVE_ASTEROID)
{
}
RemoveAsteroidMessage::~RemoveAsteroidMessage()
{
}

void RemoveAsteroidMessage::process()
{
	UNIT_MANAGER->deleteAsteroid();
}