#include "ToggleDebugMessage.h"
#include <Game.h>
#include "GameApp.h"


ToggleDebugMessage::ToggleDebugMessage()
: GameMessage(TOGGLE_DEBUG_MESSAGE)
{

}

void ToggleDebugMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (pGame != NULL)
	{
		pGame->toggleDebugMode();
	}
}