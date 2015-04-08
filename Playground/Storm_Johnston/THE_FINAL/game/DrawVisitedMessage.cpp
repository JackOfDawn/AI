#include "DrawVisitedMessage.h"
#include "Game.h"
#include "GameApp.h"
#include "GridPathfinder.h"

DrawVisitedMessage::DrawVisitedMessage()
:GameMessage(SHOW_VISITED_NODES_MESSAGE)
{
}

DrawVisitedMessage::~DrawVisitedMessage()
{
}

void DrawVisitedMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (pGame != NULL)
	{
		GridPathfinder::switchDrawVisited();
	}
}
