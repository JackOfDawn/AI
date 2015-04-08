#include "SaveMessage.h"
#include <Game.h>
#include "Editor.h"


SaveMessage::SaveMessage()
:GameMessage(SAVE_MESSAGE)
{

}

SaveMessage::~SaveMessage()
{

}

void SaveMessage::process()
{
	Editor* pEditor = dynamic_cast<Editor*>(gpGame);
	if (pEditor != NULL)
	{
		pEditor->save();
	}
}