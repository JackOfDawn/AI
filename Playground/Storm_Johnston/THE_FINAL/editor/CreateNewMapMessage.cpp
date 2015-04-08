#include "CreateNewMapMessage.h"
#include <Game.h>
#include "Editor.h"

CreateNewMapMessage::CreateNewMapMessage()
:GameMessage(CREATE_NEW_MAP_MESSAGE)
{

}

void CreateNewMapMessage::process()
{
	Editor* pEditor = dynamic_cast<Editor*>(gpGame);
	if (pEditor != NULL)
	{
		pEditor->createNewMap();
	}
}