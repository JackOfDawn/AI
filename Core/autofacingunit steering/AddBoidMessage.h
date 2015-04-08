#pragma once

#include "GameMessage.h"
#include "Vector2D.h"

class AddBoidMessage : public GameMessage
{
public:
	AddBoidMessage();
	~AddBoidMessage();

private:
	void process();
};