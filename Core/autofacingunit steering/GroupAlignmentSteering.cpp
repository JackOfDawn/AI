#include "GroupAlignmentSteering.h"
#include "KinematicUnit.h"
#include "Game.h"
#include <vector>

GroupAlignmentSteering::GroupAlignmentSteering(KinematicUnit* mover, int radius)
{
	mpMover = mover;
	mRadius = radius;
}

GroupAlignmentSteering::~GroupAlignmentSteering()
{
	delete mpMover;
}

Steering* GroupAlignmentSteering::getSteering()
{
	int totalUnits = gpGame->getUnitManager()->getSize();

	Vector2D collectiveVel = Vector2D(0, 0);
	int unitsCount = 0;

	// loop through all of the units 
	for (int i = 0; i < totalUnits; i++)
	{
		KinematicUnit* unit = gpGame->getUnitManager()->getUnit(i);

		// if this isn't the current unit add it's velocity to the collective velocity
		if (unit->getPosition().getX() != mpMover->getPosition().getX() && unit->getPosition().getY() != mpMover->getPosition().getY())
		{
			float distance = pow(unit->getPosition().getX() - mpMover->getPosition().getX(), 2) + pow(unit->getPosition().getY() - mpMover->getPosition().getY(), 2);

			distance = sqrt(distance);

			if (distance < mRadius)
			{
				unitsCount++;
				collectiveVel += unit->getVelocity();
			}
		}
	}

	// if there is at least 1 unit divide the collective velocity by the unit count
	if (unitsCount > 0)
	{
		collectiveVel /= unitsCount;
	}

	mLinear = collectiveVel;

	return this;
}