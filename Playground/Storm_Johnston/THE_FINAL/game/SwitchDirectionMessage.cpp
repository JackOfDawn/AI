#include "SwitchDirectionColor.h"
#include "MapHolder.h"
#include "GridGraph.h"
#include "GameApp.h"
#include <Game.h>

SwitchDirectionColor::SwitchDirectionColor(bool reverse)
: GameMessage(SWITCH_DIRECTION_MESSAGE)
{
	mReverse = reverse;
}

void SwitchDirectionColor::process()
{

}