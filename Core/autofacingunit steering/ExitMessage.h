#pragma once

#include "GameMessage.h"

class ExitMessage : public GameMessage
{
	public:
	ExitMessage();
	~ExitMessage();

private:
	void process();
};