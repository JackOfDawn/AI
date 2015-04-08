#include "SeparationBookSteering.h"
#include "KinematicUnit.h"
#include "Vector2D.h"
#include <vector>
#include "Defines.h"
#include "Game.h"
#include "UnitManager.h"

SeparationBookSteering::SeparationBookSteering(KinematicUnit* pMover)
{
	mpMover = pMover;
}

Steering* SeparationBookSteering::getSteering()
{
	std::vector<KinematicUnit*> neighbors = mpMover->getNeighbors();
	Vector2D direction;
	Vector2D accelerationVector = Vector2D();
	float distance;
	float strength;

	if (neighbors.size() > 0)
	{
		//loop through each of the targets
		for each (KinematicUnit* neighbor in neighbors)
		{
			//check if target is close
			direction = mpMover->getPosition() - neighbor->getPosition();
			//direction = neighbor->getPosition() - mpMover->getPosition();
			distance = direction.getLength();

			if (distance < THRESHOLD )
			{
				strength = mpMover->getMaxAcceleration() * (THRESHOLD - distance) / THRESHOLD;
				direction.normalize();
				accelerationVector += (direction * strength);
			}
		}
	}

	mLinear = accelerationVector;
	return this;
}