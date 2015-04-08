#pragma once

#include "Steering.h"
#include "VelocityMatchingSteering.h"
#include "OrientationMatchingSteering.h"

class KinematicUnit;

class GroupAlignmentSteering : public Steering
{
public:
	GroupAlignmentSteering(KinematicUnit* pMover);
	~GroupAlignmentSteering();

	Steering* getSteering();

private:
	KinematicUnit* mpMover;
	VelocityMatchingSteering* mVelocityMatch;
	OrientationMatchingSteering* mOrientaionMatch;
};