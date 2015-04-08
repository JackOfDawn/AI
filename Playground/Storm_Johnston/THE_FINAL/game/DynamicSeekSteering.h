#pragma once

#include "Steering.h"

class KinematicUnit;

class DynamicSeekSteering:public Steering
{
public:
	DynamicSeekSteering(KinematicUnit* pMover, Vector2D pTarget, bool shouldFlee = false);
	~DynamicSeekSteering(){};

	void setTarget(Vector2D pTarget){ mpTarget = pTarget; };

	virtual Steering* getSteering();

private:
	Vector2D mpTarget;
	KinematicUnit* mpMover;
	bool mShouldFlee;
	float mTargetRadius = 6;
	float mSlowRadius = 6;
	float mTimeToTarget = 10;
};