#include "SeparationSteering.h"
#include "Game.h"
#include "Kinematic.h"

SeparationSteering::SeparationSteering(KinematicUnit* mover, int radius, int decay, int maxAcc)
	: Steering(gZeroVector2D, 0.0f, false, false)
{
	mpMover = mover;
	mRadius = radius;
	mDecay = decay;
	mMaxAcceleration = maxAcc;
}

SeparationSteering::~SeparationSteering()
{
	delete mpMover;
}

Steering* SeparationSteering::getSteering()
{
	int totalUnits = gpGame->getUnitManager()->getSize();

	// loop through all the units and find out which ones are close to us
	// then figure out how to separate from those units that are close enough
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