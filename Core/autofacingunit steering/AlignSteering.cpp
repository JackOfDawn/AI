#include "AlignSteering.h"
#include "KinematicUnit.h"
#include "Game.h"

AlignSteering::AlignSteering(KinematicUnit *pMover, float targetOrientation, float targetRadius, float slowRadius, float timeToTarget)
:mpMover(pMover)
,mTargetOrientation(targetOrientation)
,mTargetRadius(targetRadius)
,mSlowRadius(slowRadius)
,mTimeToTarget(timeToTarget)
{
	mApplyLinearDirectly = false;
	mApplyAngularDirectly = true;
}

Steering* AlignSteering::getSteering()
{
	//get niave direction to rotate
	float rotation = mTargetOrientation - mpMover->getOrientation();

	//map to (-pi, pi) interval
	rotation = mapRotationToRange( rotation, -PI, PI );
	float rotationSize = abs(rotation);

	//are we there yet?
	if( rotationSize < mTargetRadius )
	{
		mLinear = gZeroVector2D;
		mAngular = 0.0f;

		return this;
	}

	float targetRotation = 0.0f;

	//are we outside slow radius?
	if( rotationSize > mSlowRadius )
	{
		targetRotation = mpMover->getMaxRotationalVelocity();
	}
	else
	{
		targetRotation = ( mpMover->getMaxRotationalVelocity() * rotationSize ) / mSlowRadius;
	}

	//combine to get targetRotation
	targetRotation *= rotation / rotationSize;

	//set angular acceleration
	mAngular = targetRotation - mpMover->getOrientation();

	//check if too fast
	float angularAcceleration = abs(mAngular);
	if( angularAcceleration > mpMover->getMaxAngularAcceleration() )//fast - move it back down to max
	{
		mAngular /= angularAcceleration;
		mAngular *= mpMover->getMaxAngularAcceleration();
	}

	//no linear acceleration
	mLinear = gZeroVector2D;

	return this;
}