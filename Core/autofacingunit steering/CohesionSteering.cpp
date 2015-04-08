#include "CohesionSteering.h"
#include "Game.h"
#include <vector>

CohesionSteering::CohesionSteering(KinematicUnit* mover, int radius, int decay, int maxAcceleration)
{
	mpMover = mover;
	mRadius = radius;
	mDecay = decay;
	mMaxAcceleration = maxAcceleration;
}

CohesionSteering::~CohesionSteering()
{
	delete mpMover;
}

Steering* CohesionSteering::getSteering()
{
	int totalUnits = gpGame->getUnitManager()->getSize();

	// loop through all the units and find the ones close enough
	// find the linear velocity for staying with this pack
	for (int i = 0; i < totalUnits; i++)
	{
		KinematicUnit* unit = gpGame->getUnitManager()->getUnit(i);

		if (unit->getPosition().getX() != mpMover->getPosition().getX() && unit->getPosition().getY() != mpMover->getPosition().getY())
		{
			Vector2D direction = unit->getPosition() - mpMover->getPosition();

			float distance = pow(unit->getPosition().getX() - mpMover->getPosition().getX(), 2) + pow(unit->getPosition().getY() - mpMover->getPosition().getY(), 2);

			distance = sqrt(distance);

			if (distance < mRadius)
			{
				float strength = mDecay / (float)distance;

				if (strength > mMaxAcceleration)
				{
					strength = mMaxAcceleration;
				}

				direction.normalize();
				mLinear += Vector2D((strength * direction.getX()), (strength * direction.getY()));
			}
		}
	}

	return this;
}