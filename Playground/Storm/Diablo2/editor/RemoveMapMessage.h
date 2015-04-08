#pragma once
#include "GameMessage.h"

class RemoveMapMessage : public GameMessage
{
public:
	RemoveMapMessage();
	~RemoveMapMessage();

	void process();
private:
};