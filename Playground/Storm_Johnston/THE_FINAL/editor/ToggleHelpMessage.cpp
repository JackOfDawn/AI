#include "ToggleHelpMessage.h"
#include <Game.h>
#include "Editor.h"
#include "HUD.h"

ToggleHelpMessage::ToggleHelpMessage()
: GameMessage(TOGGLE_HELP_MESSAGE)
{

}

void ToggleHelpMessage::process()
{
	HUD* pHUD= dynamic_cast<Editor*>(gpGame)->getHUD();
	if (pHUD != NULL)
	{
		pHUD->toggleHelp();
	}
}