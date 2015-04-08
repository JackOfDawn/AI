#pragma once
#include "GameMessage.h"

class AddAsteroidMessage : public GameMessage
{
public:
	AddAsteroidMessage(int type);
	~AddAsteroidMessage();

	void process();

private:
	int mType;
};
