#pragma once
#include "GameMessage.h"

class RemoveAsteroidMessage : public GameMessage
{
public:
	RemoveAsteroidMessage();
	~RemoveAsteroidMessage();

	void process();
};