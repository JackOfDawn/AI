#pragma once
#include "GameMessage.h"
#include "MapDefines.h"

class GodModeMessage : public GameMessage
{
public:
	GodModeMessage();
	~GodModeMessage(){};

	void process();

private:

};