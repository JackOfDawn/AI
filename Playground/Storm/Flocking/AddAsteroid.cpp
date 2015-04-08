#include "AddAsteroidMessage.h"
#include "Defines.h"
#include "Game.h"
#include "GameMessageManager.h"
#include "UnitManager.h"

AddAsteroidMessage::AddAsteroidMessage(int type)
:GameMessage(ADD_ASTEROID)
, mType(type)
{
}

AddAsteroidMessage::~AddAsteroidMessage()
{
}
void AddAsteroidMessage::process()
{
	UNIT_MANAGER->createAsteroid(mType);
}