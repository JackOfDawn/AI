#include "RemoveMapMessage.h"
#include <Game.h>
#include "Editor.h"

RemoveMapMessage::RemoveMapMessage()
: GameMessage(REMOVE_MAP_MESSAGE)
{

}

void RemoveMapMessage::process()
{
	Editor* pEditor = dynamic_cast<Editor*>(gpGame);
	if (pEditor != NULL)
	{
		pEditor->removeCurrentMap();
	}
}