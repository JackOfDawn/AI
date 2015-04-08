#pragma once
#include "GameMessage.h"

class MakeEndPointMessage : public GameMessage
{
public:
	MakeEndPointMessage(int x, int y);
	~MakeEndPointMessage(){};

	void process();

private:
	int mX;
	int mY;
};