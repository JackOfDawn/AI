#include <string>
#include <sstream>
#include "AutoFacingUnitDebugContent.h"
#include "AutoFacingUnit.h"

using namespace std;

AutoFacingUnitDebugContent::AutoFacingUnitDebugContent( AutoFacingUnit* pUnit )
	:mpUnit(pUnit)
{
}

string AutoFacingUnitDebugContent::getDebugString()
{
	stringstream theStream;
	theStream << mpUnit->getRotation() << " " << mpUnit->getOrientation() << " " << mpUnit->mpFaceDirectionSteering;

	return theStream.str();
}