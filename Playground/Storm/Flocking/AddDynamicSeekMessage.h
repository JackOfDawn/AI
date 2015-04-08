#pragma once
#include "GameMessage.h"

class AddDynamicSeekMessage : public GameMessage
{
public:
	AddDynamicSeekMessage(float distance);
	~AddDynamicSeekMessage();

	void process();

private:
	float mDistance;
};