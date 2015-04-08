#include "AdjustWeightMessage.h"
#include "Defines.h"
#include "Game.h"
#include "GUI.h"

AdjustWeightMessage::AdjustWeightMessage(float amount)
: GameMessage(ADJUST_WEIGHT_MESSAGE)
{
	mAmount = amount;
}

void AdjustWeightMessage::process()
{
	GAME_GUI->adjustType(mAmount);
}