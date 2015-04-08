#pragma once
#include "GameMessage.h"

class SelectTypeMessage : public GameMessage
{
public: 
	SelectTypeMessage(float type);
	~SelectTypeMessage(){};

	void process();

private:
	float mType;

};