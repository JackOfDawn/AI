#include "AddFlockMessage.h"
#include "UnitManager.h"
#include "Defines.h"
#include "Game.h"

AddFlockMessage::AddFlockMessage()
:GameMessage(ADD_FLOCK){}

AddFlockMessage::~AddFlockMessage(){}

void AddFlockMessage::process()
{
	UNIT_MANAGER->addFlock();
}

