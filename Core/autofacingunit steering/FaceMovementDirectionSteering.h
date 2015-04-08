#pragma once

#include "Steering.h"
#include "AlignSteering.h"

class KinematicUnit;

class FaceMovementDirectionSteering:public AlignSteering
{
public:
	FaceMovementDirectionSteering( KinematicUnit* pMover, float targetRadius = 0.1f, float slowRadius = 0.5f, float timeToTarget = 0.25f );
	~FaceMovementDirectionSteering(){};

	virtual Steering* getSteering();

private:
};