#include "OrientationMatchingSteering.h"
#include "KinematicUnit.h"
#include "Vector2D.h"
#include <vector>
#include "Defines.h"
#include "Game.h"
#include "UnitManager.h"

OrientationMatchingSteering::OrientationMatchingSteering(KinematicUnit* pMover)
{
	mpMover = pMover;
}
Steering* OrientationMatchingSteering::getSteering()
{
	std::vector<KinematicUnit*> neighbors = mpMover->getNeighbors();

	if (neighbors.size() > 0)
	{
		float orientation = matchOrientation(neighbors);
		
		mAngular = orientation;
	}

	return this;
}

float OrientationMatchingSteering::matchOrientation(std::vector<KinematicUnit*>& neighbors)
{
	float averageOrientation = 0.0f;

	for each (KinematicUnit* neighbor in neighbors)
	{
		averageOrientation += neighbor->getOrientation();
	}

	averageOrientation /= neighbors.size();

	return averageOrientation;
}