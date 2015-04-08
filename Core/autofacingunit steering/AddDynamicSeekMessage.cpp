#include "AddDynamicSeekMessage.h"
#include "Game.h"


AddDynamicSeekMessage::AddDynamicSeekMessage()
	: GameMessage(ADD_DYNAMIC_SEEK)
{
}

AddDynamicSeekMessage::~AddDynamicSeekMessage()
{
}

void AddDynamicSeekMessage::process()
{
	// add a new AI unit that dynamically arrives at the player
	Vector2D playerPos = gpGame->getUnitManager()->getUnit(0)->getPosition();
	Vector2D AIPos;

	Vector2D screenSize(gpGame->GetWidth(), gpGame->GetHeight());

	if (playerPos.getX() - 100 >= 0)
	{
		// spawn AI unit here
		AIPos = Vector2D(playerPos.getX() - 100, playerPos.getY());
	}
	else if (playerPos.getY() - 100 >= 0)
	{
		// spawn AI unit here
		AIPos = Vector2D(playerPos.getX(), playerPos.getY() - 100);
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

	// create the unit
	KinematicUnit* unit = new KinematicUnit(gpGame->getArrowSprite(), AIPos, 1, vel, 0.0f, 200.0f, 10.0f);

	// add the unit to the unit manager
	gpGame->getUnitManager()->addUnit(unit);

	// give the unit the dynamic seek behavior
	unit->dynamicSeek(gpGame->getUnitManager()->getUnit(0));
}