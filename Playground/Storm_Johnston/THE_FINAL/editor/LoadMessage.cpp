#include "LoadMessage.h"
#include <Game.h>
#include "Editor.h"

LoadMessage::LoadMessage()
:GameMessage(LOAD_MESSAGE)
{

}

LoadMessage::~LoadMessage(){};

void LoadMessage::process()
{
	Editor* pEditor = dynamic_cast<Editor*>(gpGame);
	if (pEditor != NULL)
	{
		pEditor->load();
	}
}