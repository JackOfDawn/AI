#pragma once
#include "GameMessage.h"

class AddWanderFleeMessage : public GameMessage
{
public:
	AddWanderFleeMessage(float distance);
	~AddWanderFleeMessage();

	void process();

private:
	float mDistance;

};
