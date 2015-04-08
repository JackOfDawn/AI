#pragma once
#include "GameMessage.h"
#include "MapDefines.h"

class SetEndPointMessage : public GameMessage
{
public:
	SetEndPointMessage(EndPoint endPoint);
	~SetEndPointMessage(){};

	void process();

private:
	EndPoint mEndPoint;
};