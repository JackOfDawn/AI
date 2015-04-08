#include "DynamicWanderSteering.h"
#include "KinematicUnit.h"
#include "Game.h"

DynamicWanderSteering::DynamicWanderSteering(KinematicUnit* pMover)
{
	mpMover = pMover;
	mWanderOrientation = genRandomBinomial() * MAX_DYNAMIC_WANDER_ROTATION;
}

Steering* DynamicWanderSteering::getSteering()
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

	mAngular = 0;
	mLinear.normalize();
	mLinear *= mpMover->getMaxVelocity();

	return this;
}

Vector2D DynamicWanderSteering::asVector(float orientation)
{
	return Vector2D(sinf(orientation), cosf(orientation));
}