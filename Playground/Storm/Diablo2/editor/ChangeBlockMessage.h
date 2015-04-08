#pragma once
#include "GameMessage.h"

class ChangeBlockMessage : public GameMessage
{
public:
	ChangeBlockMessage(int blockID);
	~ChangeBlockMessage(){};

	void process();

private:
	int mBlockID;
};