#pragma once

#include "DebugContent.h"

class AutoFacingUnit;

class AutoFacingUnitDebugContent:public DebugContent
{
public:
	AutoFacingUnitDebugContent( AutoFacingUnit* pUnit );
	~AutoFacingUnitDebugContent(){};

	std::string getDebugString();

private:
	AutoFacingUnit* mpUnit;
};