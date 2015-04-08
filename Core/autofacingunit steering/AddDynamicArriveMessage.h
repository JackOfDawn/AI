#pragma once

#include "GameMessage.h"
#include "Vector2D.h"

class AddDynamicArriveMessage : public GameMessage
{
public:
	AddDynamicArriveMessage();
	~AddDynamicArriveMessage();

private:
	void process();
};