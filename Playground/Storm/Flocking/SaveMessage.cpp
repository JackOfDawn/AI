#include "SaveMessage.h"
#include "Game.h"
SaveMessage::SaveMessage()
:GameMessage(SAVE_MESSAGE)
{

}

void SaveMessage::process()
{
	gpGame->saveParameters();
}