#pragma once

#include "GameMessage.h"
#include "Vector2D.h"

class AddDynamicSeekMessage : public GameMessage
{
public:
	AddDynamicSeekMessage();
	~AddDynamicSeekMessage();

private:
	void process();
};