#pragma once
#include "GameMessage.h"
#include "MapDefines.h"

class JumpToPlayerMapMessage : public GameMessage
{
public:
	JumpToPlayerMapMessage();
	~JumpToPlayerMapMessage(){};

	void process();

private:

};