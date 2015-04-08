#pragma once

#include "GameMessage.h"
#include "Vector2D.h"

class RemoveBoidMessage : public GameMessage
{
public:
	RemoveBoidMessage();
	~RemoveBoidMessage();

private:
	void process();
};