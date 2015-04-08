#include "DeleteUnitMessage.h"
#include "Game.h"


DeleteUnitMessage::DeleteUnitMessage()
	: GameMessage(DELETE_UNIT)
{
}

DeleteUnitMessage::~DeleteUnitMessage()
{
}

void DeleteUnitMessage::process()
{
	// delete the last unit from the unit manager
	gpGame->getUnitManager()->removeUnit(gpGame->getUnitManager()->getSize() - 1);

}