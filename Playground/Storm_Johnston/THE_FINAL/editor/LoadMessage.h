#pragma once

#include "GameMessage.h"

class LoadMessage : public GameMessage
{
public:
	LoadMessage();
	~LoadMessage();

	void process();
};