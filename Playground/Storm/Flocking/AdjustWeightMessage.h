#pragma once
#include "GameMessage.h"

class AdjustWeightMessage : public GameMessage
{
public:
	AdjustWeightMessage(float amount);
	~AdjustWeightMessage(){};

	void process();

private:
	float mAmount;

};