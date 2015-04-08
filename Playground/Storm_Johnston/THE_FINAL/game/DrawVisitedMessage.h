#pragma once

#include "GameMessage.h"


class DrawVisitedMessage :public GameMessage
{
public:
	DrawVisitedMessage();
	~DrawVisitedMessage();

	void process();

private:
	int mId;
};