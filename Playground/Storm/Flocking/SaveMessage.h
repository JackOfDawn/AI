#pragma once
#include "GameMessage.h"

class SaveMessage : public GameMessage
{
public:
	SaveMessage();
	~SaveMessage(){};

	void process();

};