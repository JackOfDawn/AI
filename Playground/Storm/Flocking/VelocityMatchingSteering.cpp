#include "VelocityMatchingSteering.h"
#include "KinematicUnit.h"
#include "Defines.h"
#include "Game.h"
#include <vector>
#include "Vector2D.h"
#include "UnitManager.h"

VelocityMatchingSteering::VelocityMatchingSteering(KinematicUnit* pMover)
{
	mpMover = pMover;
}

Steering* VelocityMatchingSteering::getSteering()
{
	std::vector<KinematicUnit*> neighbors = mpMover->getNeighbors();

	if (neighbors.size() > 0)
	{
		Vector2D velocityVector = matchVelocity(neighbors);

		mLinear = velocityVector;
	}

	return this;
}

Vector2D VelocityMatchingSteering::matchVelocity(const std::vector<KinematicUnit*>& neighbors)
{

	Vector2D averageVelocity = Vector2D();
	for each  (KinematicUnit* neighbor in neighbors)
	{
		averageVelocity += neighbor->getVelocity();
	}
	averageVelocity /= neighbors.size();

	return averageVelocity;
}