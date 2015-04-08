#include "DynamicSeekSteering.h"
#include "KinematicUnit.h"

DynamicSeekSteering::DynamicSeekSteering(KinematicUnit *pMover, Vector2D pTarget, bool shouldFlee)
:mpMover(pMover)
,mpTarget(pTarget)
,mShouldFlee(shouldFlee)
{
	mApplyDirectly = false;
}

Steering* DynamicSeekSteering::getSteering()
{
	mLinear = mpTarget - mpMover->getPosition();

	if (mLinear != Vector2D())
		mLinear.normalize();
	mLinear *= mpMover->getMaxAcceleration();
	mAngular = 0;
	return this;
}