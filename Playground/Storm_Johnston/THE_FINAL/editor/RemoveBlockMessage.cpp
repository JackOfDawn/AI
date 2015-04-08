#include "RemoveBlockMessage.h"
#include <Game.h>
#include "Editor.h"

RemoveBlockMessage::RemoveBlockMessage(int x, int y)
:GameMessage(REMOVE_BLOCK_MESSAGE)
, mX(x)
, mY(y)
{}

void RemoveBlockMessage::process()
{
	Editor* pEditor = dynamic_cast<Editor*>(gpGame);
	if (pEditor != NULL)
	{
		pEditor->removeBlock(mX, mY);
	}
}
