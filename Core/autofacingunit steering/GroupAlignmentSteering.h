#pragma once

#include "Steering.h"
#include "KinematicUnit.h"

class GroupAlignmentSteering : public Steering
{
public:
	GroupAlignmentSteering(KinematicUnit* mover, int radius);
	~GroupAlignmentSteering();

	Steering* getSteering();

private:
	KinematicUnit* mpMover;
	int mRadius;
};