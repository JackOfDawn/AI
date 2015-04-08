#include "Game.h"
#include "DisplayMouseCoordMessage.h"
#include "GameMessageManager.h"
#include <sstream>

DisplayMouseCoordMessage::DisplayMouseCoordMessage(Vector2D &pos)
:GameMessage(DISPLAY_MOUSE_COORD_MESSAGE)
, mPos(pos)
{
}

DisplayMouseCoordMessage::~DisplayMouseCoordMessage()
{
}

void DisplayMouseCoordMessage::process()
{
	gpGame->setMouseLocation(mPos);
}