#pragma once

#include "GameMessage.h"


class ChangeAlgorithmMessage :public GameMessage
{
public:
	ChangeAlgorithmMessage(int id);
	~ChangeAlgorithmMessage();

	void process();

private:
	int mId;
};