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


	mLinear.normalize();
	mLinear *= mpMover->getMaxVelocity();
	mAngular = 0;
	return this;

}