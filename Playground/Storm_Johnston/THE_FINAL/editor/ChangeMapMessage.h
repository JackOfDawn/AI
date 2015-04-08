#pragma once
#include "GameMessage.h"

class ChangeMapMessage : public GameMessage
{
public:
	ChangeMapMessage(bool forward);
	~ChangeMapMessage() {};

	void process();
private:
	bool mIsForward;
};