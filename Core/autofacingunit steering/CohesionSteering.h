#pragma once

#include "Steering.h"
#include "KinematicUnit.h"

class CohesionSteering : public Steering
{
public:
	CohesionSteering(KinematicUnit* mover, int radius, int decay, int maxAcceleration);
	~CohesionSteering();

	Steering* getSteering();

private:
	KinematicUnit* mpMover;
	int mRadius;
	int mDecay;
	int mMaxAcceleration;
};