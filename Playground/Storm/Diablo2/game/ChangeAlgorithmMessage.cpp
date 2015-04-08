#include "ChangeAlgorithmMessage.h"
#include "Game.h"
#include "GameApp.h"
#include "GridPathfinder.h"

ChangeAlgorithmMessage::ChangeAlgorithmMessage(int id)
:GameMessage(CHANGE_ALGORITHM_MESSAGE)
, mId(id)
{
}

ChangeAlgorithmMessage::~ChangeAlgorithmMessage()
{
}

void ChangeAlgorithmMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (pGame != NULL)
	{
		
	}
}
