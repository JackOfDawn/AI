#pragma once
#include "GameMessage.h"

class AddDynamicArriveMessage : public GameMessage
{
public:
	AddDynamicArriveMessage(float distance);
	~AddDynamicArriveMessage();

	void process();

private:
	float mDistance;//note - in pixels
};
