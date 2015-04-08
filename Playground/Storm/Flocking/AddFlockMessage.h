#pragma once
#include "GameMessage.h"

class AddFlockMessage : public GameMessage
{
public:
	AddFlockMessage();
	~AddFlockMessage();

	void process();
};