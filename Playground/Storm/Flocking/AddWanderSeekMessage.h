#pragma once
#include "GameMessage.h"

class AddWanderSeekMessage : public GameMessage
{
public:
	AddWanderSeekMessage(float distance);
	~AddWanderSeekMessage();

	void process();

private:
	float mDistance;

};
