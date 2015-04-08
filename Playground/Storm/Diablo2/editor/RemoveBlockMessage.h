#pragma once
#include "GameMessage.h"

class RemoveBlockMessage : public GameMessage
{
public:
	RemoveBlockMessage(int x, int y);
	~RemoveBlockMessage(){};

	void process();
private:
	int mX;
	int mY;
};