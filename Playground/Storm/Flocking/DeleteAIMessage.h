#pragma once
#include "GameMessage.h"

class DeleteAIMessage : public GameMessage
{
public:
	DeleteAIMessage();
	~DeleteAIMessage();

	void process();
};