#pragma once
#include "GameMessage.h"

class SwitchDirectionColor : public GameMessage
{
public:
	SwitchDirectionColor(bool reverse);
	~SwitchDirectionColor(){};

	void process();

private:
	bool mReverse;
};