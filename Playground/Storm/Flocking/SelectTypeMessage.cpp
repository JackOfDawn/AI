#include "SelectTypeMessage.h"
#include "Defines.h"
#include "Game.h"
#include "GUI.h"

SelectTypeMessage::SelectTypeMessage(float type)
: GameMessage(SELECT_TYPE_MESSAGE)
{
	mType = type;
}

void SelectTypeMessage::process()
{
	GAME_GUI->selectType(mType);
}