#include "ChangeBlockMessage.h"
#include <Game.h>
#include "Editor.h"

ChangeBlockMessage::ChangeBlockMessage(int blockID)
: GameMessage(CHANGE_BLOCK_TYPE)
, mBlockID(blockID)
{}

void ChangeBlockMessage::process()
{
	Editor* pEditor = dynamic_cast<Editor*>(gpGame);
	if (pEditor != NULL)
	{
		pEditor->setBlockType(mBlockID);
	}
}