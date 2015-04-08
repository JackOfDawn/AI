#include "SeparationSteering.h"
#include "KinematicUnit.h"
#include "Vector2D.h"
#include <vector>
#include "Defines.h"
#include "Game.h"
#include "UnitManager.h"

SeparationSteering::SeparationSteering(KinematicUnit* pMover)
{
	mpMover = pMover;
}

SeparationSteering::~SeparationSteering()
{

}


Steering* SeparationSteering::getSteering()
{
	std::vector<KinematicUnit*> neighbors = mpMover->getNeighbors();

	if (neighbors.size() > 0)
	{
		Vector2D seperationVector = getSeperationVector(neighbors);

		mLinear = seperationVector;
	}

	return this;
}

Vector2D SeparationSteering::getSeperationVector(const std::vector<KinematicUnit*>& neighbors)
{
	Vector2D seperationVect = Vector2D();

	for each (KinematicUnit* neighbor in neighbors)
	{
		//add the separations of neighboring units
		seperationVect += (neighbor->getPosition() - mpMover->getPosition());
	}

	seperationVect /= neighbors.size();
	seperationVect *= -1;
	
	return seperationVect;
}
