#include "VelocityMatching.h"

VelocityMatching::VelocityMatching(KinematicUnit* mover, Vector2D targetVelocity, float timeToTarget, float maxAcceleration)
	: Steering(gZeroVector2D, 0.0f, false, false)
{
	mpMover = mover;
	mTargetVelocity = targetVelocity;
	mTimeToTarget = timeToTarget;
	mMaxAcceleration = maxAcceleration;
}

VelocityMatching::~VelocityMatching()
{
	delete mpMover;
}

Steering* VelocityMatching::getSteering()
{
	// match the target velocity
	mLinear = mTargetVelocity - mpMover->getVelocity();
	mLinear /= mTimeToTarget;

	if (mLinear.getLength() > mMaxAcceleration)
	{
		mLinear.normalize();
		mLinear *= mMaxAcceleration;
	}

	mAngular = 0;

	return this;
}