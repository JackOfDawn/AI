#include "WanderAndSeekSteering.h"
#include "KinematicUnit.h"
#include "Game.h"


WanderAndSeekSteering::WanderAndSeekSteering(KinematicUnit* pMover, KinematicUnit* pTarget, bool shouldFlee)
:mpMover(pMover)
,mpTarget(pTarget)
,mShouldFlee(shouldFlee)
{
	mApplyDirectly = false;
	mShouldWander = true;
	mWanderOrientation = genRandomBinomial() * MAX_DYNAMIC_WANDER_ROTATION;
}

Steering* WanderAndSeekSteering::getSteering()
{

		mApplyDirectly = false;
		checkIfShouldWander();

		//wander
		if (mShouldWander)
		{
			wander();
		}
		else
		{
			seek();
		}

		
		mAngular = 0;
	//}


	mLinear.normalize();
	mLinear *= mpMover->getMaxVelocity();

	return this;
}


void WanderAndSeekSteering::wander()
{
	//1.calculate the target to delegate to face

	//update wander orientation 
	mWanderOrientation += genRandomBinomial() * MAX_DYNAMIC_WANDER_ROTATION;

	//calculate the combined target orientation
	float	targetOrientation = mWanderOrientation + mpMover->getOrientation();

	//Calculate center of wander circle

	Vector2D target = mpMover->getPosition() + (mpMover->getOrientationAsVector() * WANDER_FORWARD_OFFSET);

	//calculate target location
	Vector2D targetOffset = (asVector(targetOrientation) * WANDER_RADIUS);
	target = target + targetOffset;

	//set the acceleration to be at full
	mLinear = target - mpMover->getPosition();

	

}


Vector2D WanderAndSeekSteering::asVector(float orientation)
{
	return Vector2D(sinf(orientation), cosf(orientation));
}

void WanderAndSeekSteering::seek()
{
	if (!mShouldFlee)
	{
		mLinear = mpTarget->getPosition() - mpMover->getPosition();
	}
	else
	{
		mLinear = mpMover->getPosition() - mpTarget->getPosition();
	}	

}

void WanderAndSeekSteering::checkIfShouldWander()
{
	float	distanceX = (mpMover->getPosition().getX() - mpTarget->getPosition().getX()),
			distanceY = (mpMover->getPosition().getY() - mpTarget->getPosition().getY());

	bool isCloseEnough = (distanceX*distanceX) + (distanceY*distanceY) < MINIMUM_DISTANCE * MINIMUM_DISTANCE;

	if (isCloseEnough) mShouldWander = false;
	else mShouldWander = true;
}