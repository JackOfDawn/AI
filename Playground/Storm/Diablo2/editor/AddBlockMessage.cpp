#include "AddBlockMessage.h"
#include <Game.h>
#include "Editor.h"

AddBlockMessage::AddBlockMessage(int x, int y)
: GameMessage(ADD_BLOCK_MESSAGE)
, mX(x)
, mY(y)
{}

void AddBlockMessage::process()
{
	Editor* pEditor = dynamic_cast<Editor*>(gpGame);
	if (pEditor != NULL)
	{
		pEditor->addBlock(mX, mY);
	}
}