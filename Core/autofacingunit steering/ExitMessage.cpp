#include "ExitMessage.h"
#include "Game.h"

ExitMessage::ExitMessage()
	: GameMessage(EXIT_MESSAGE)
{
}

ExitMessage::~ExitMessage()
{
}

void ExitMessage::process()
{
	gpGame->setShouldExit(true);
}