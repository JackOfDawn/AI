#pragma once

#include "GameMessage.h"
#include "Vector2D.h"

class DeleteUnitMessage : public GameMessage
{
public:
	DeleteUnitMessage();
	~DeleteUnitMessage();

private:
	void process();
};