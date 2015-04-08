#pragma once
#include "GameMessage.h"

class CreateNewMapMessage : public GameMessage
{
public:
	CreateNewMapMessage();
	~CreateNewMapMessage(){};

	void process();

private:
};