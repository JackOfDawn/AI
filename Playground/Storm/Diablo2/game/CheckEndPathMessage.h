#pragma once

#include "GameMessage.h"

class CheckPathEndMessage : public GameMessage
{
public:
	CheckPathEndMessage();
	~CheckPathEndMessage(){};

	void process();

private:
};
