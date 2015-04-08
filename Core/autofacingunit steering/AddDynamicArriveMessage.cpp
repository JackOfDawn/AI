#include "AddDynamicArriveMessage.h"
#include "Game.h"


AddDynamicArriveMessage::AddDynamicArriveMessage()
	: GameMessage(ADD_DYNAMIC_MOVE)
{
}

AddDynamicArriveMessage::~AddDynamicArriveMessage()
{
}

void AddDynamicArriveMessage::process()
{
	// add a new AI unit that dynamically arrives at the player
	Vector2D playerPos = gpGame->getUnitManager()->getUnit(0)->getPosition();
	Vector2D AIPos;

	Vector2D screenSize(gpGame->GetWidth(), gpGame->GetHeight());

	if (playerPos.getX() - 200 >= 0)
	{
		// spawn AI unit here
		AIPos = Vector2D(playerPos.getX() - 200, playerPos.getY());
	}
	else if (playerPos.getY() - 200 >= 0)
	{
		// spawn AI unit here
		AIPos = Vector2D(playerPos.getX(), playerPos.getY() - 200);
	}
	else if (playerPos.getX() + 100 <= screenSize.getX())
	{
		// spawn AI unit here
		AIPos = Vector2D(playerPos.getX() + 100, playerPos.getY());
	}
	else if (playerPos.getY() + 100 <= screenSize.getY())
	{
		// spawn AI unit here
		AIPos = Vector2D(playerPos.getX(), playerPos.getY() + 100);
	}

	Vector2D vel(0.0f, 0.0f);

	// create unit
	KinematicUnit* unit = new KinematicUnit(gpGame->getArrowSprite(), AIPos, 1, vel, 0.0f, 180.0f, 100.0f);

	// add unit to unit manager
	gpGame->getUnitManager()->addUnit(unit);

	// add dynamic arrive behavior
	unit->dynamicArrive(gpGame->getUnitManager()->getUnit(0));
}