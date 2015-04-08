#pragma once

#include "Steering.h"

class KinematicUnit;

class AlignSteering:public Steering
{
public:
	AlignSteering( KinematicUnit* pMover, float targetOrientation, float targetRadius = 0.1f, float slowRadius = 0.5f, float timeToTarget = 0.25f );
	~AlignSteering(){};

	void setTarget( float targetOrientation ){ mTargetOrientation = targetOrientation; };

	virtual Steering* getSteering();

protected:
	KinematicUnit* mpMover;
	float mTargetOrientation;
	float mTargetRadius;
	float mSlowRadius;
	float mTimeToTarget;
};