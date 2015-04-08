#include "ToggleHelpMessage.h"
#include <Game.h>
#include "GameApp.h"
#include "HUD.h"

ToggleHelpMessage::ToggleHelpMessage()
: GameMessage(TOGGLE_HELP_MESSAGE)
{

}

void ToggleHelpMessage::process()
{
	HUD* pHUD= dynamic_cast<GameApp*>(gpGame)->getHUD();
	if (pHUD != NULL)
	{
		pHUD->toggleHelp();
	}
}