#include "GroupAlignmentSteering.h"
#include "KinematicUnit.h"
#include "Vector2D.h"
#include <vector>
#include "Defines.h"
#include "Game.h"
#include "UnitManager.h"

GroupAlignmentSteering::GroupAlignmentSteering(KinematicUnit* pMover)
{
	mpMover = pMover;
	mVelocityMatch = new VelocityMatchingSteering(pMover);
	mOrientaionMatch = new OrientationMatchingSteering(pMover);
}

GroupAlignmentSteering::~GroupAlignmentSteering()
{
	delete mVelocityMatch;
	delete mOrientaionMatch;
}

Steering* GroupAlignmentSteering::getSteering()
{
	std::vector<KinematicUnit*> neighbors = mpMover->getNeighbors();

	if (neighbors.size() > 0)
	{
		mVelocityMatch->getSteering();
		mOrientaionMatch->getSteering();

		Vector2D velocityVector = mVelocityMatch->getLinear();
		mLinear = velocityVector;

		float averageOrientaion = mOrientaionMatch->getAngular();
		mAngular = averageOrientaion;
	}
	return this;
}
