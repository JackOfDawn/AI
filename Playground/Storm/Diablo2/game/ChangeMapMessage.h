#pragma once
#include "GameMessage.h"

class ChangeMapMessage : public GameMessage
{
public:
	ChangeMapMessage(int x, int y, bool leftClick);
	~ChangeMapMessage(){};

	void process();

private:
	int mX;
	int mY;
	bool mLeftClick;
};