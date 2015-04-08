#pragma once

#include "GameMessage.h"

class AddBlockMessage : public GameMessage
{
public:
	AddBlockMessage(int x, int y);
	~AddBlockMessage(){};

	void process();
private:
	int mX;
	int mY;

};