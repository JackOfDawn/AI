#pragma once
#include "GameMessage.h"

class ToggleHelpMessage : public GameMessage
{
public:
	ToggleHelpMessage();
	~ToggleHelpMessage(){};

	void process();
};