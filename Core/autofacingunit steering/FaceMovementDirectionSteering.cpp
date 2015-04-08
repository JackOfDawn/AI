#include "AlignSteering.h"
#include "FaceMovementDirectionSteering.h"
#include "KinematicUnit.h"

FaceMovementDirectionSteering::FaceMovementDirectionSteering(KinematicUnit *pMover, float targetRadius, float slowRadius, float timeToTarget)
:AlignSteering(pMover, pMover->getOrientation(), targetRadius, slowRadius, timeToTarget)
{
	mApplyLinearDirectly = false;
	mApplyAngularDirectly = true;
}

Steering* FaceMovementDirectionSteering::getSteering()
{
	if( mpMover->getVelocity().getLengthSquared() < 0.001f )//stopped
	{
		mTargetOrientation = mpMover->getOrientation();//set to current orientation as default
	}
	else
	{
		mTargetOrientation = Kinematic::getOrientationFromVelocity( mpMover->getOrientation(), mpMover->getVelocity() );
	}

	//let superclass figure out how to align
	return AlignSteering::getSteering();
}