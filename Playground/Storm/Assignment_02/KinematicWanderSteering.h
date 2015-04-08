#pragma once

#include "Steering.h"

class KinematicUnit;

const float MAX_WANDER_ROTATION = .2f;

class KinematicWanderSteering:public Steering
{
public:
	KinematicWanderSteering( KinematicUnit* pMover );
	~KinematicWanderSteering(){};

	virtual Steering* getSteering();

private:
	KinematicUnit* mpMover;
};