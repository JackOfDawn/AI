#include "ChangeMapMessage.h"
#include <Game.h>
#include "Editor.h"

ChangeMapMessage::ChangeMapMessage(bool forward)
:GameMessage(CHANGE_MAP_MESSAGE)
, mIsForward(forward)
{

}

void ChangeMapMessage::process()
{
	Editor* pEditor = dynamic_cast<Editor*>(gpGame);
	if (pEditor != NULL)
	{
		pEditor->changeMap(mIsForward);
	}
}

