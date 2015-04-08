#include "CohesionSteering.h"
#include "KinematicUnit.h"
#include "Vector2D.h"
#include <vector>
#include "Defines.h"
#include "Game.h"
#include "UnitManager.h"

CohesionSteering::CohesionSteering(KinematicUnit* pMover)
{
	mpMover = pMover;
}

CohesionSteering::~CohesionSteering()
{

}

Steering* CohesionSteering::getSteering()
{
	std::vector<KinematicUnit*> neighbors = mpMover->getNeighbors();

	if (neighbors.size() <= 0)
	{
		return this;
	}
	else
	{
		Vector2D cohesionVector = getCohesion(neighbors);

		mLinear = cohesionVector - mpMover->getPosition();

		return this;
	}
}

Vector2D CohesionSteering::getCohesion(const std::vector<KinematicUnit*>& neighbors)
{
	Vector2D cohesionVect = Vector2D();
	for each (KinematicUnit* neighbor in neighbors)
	{
		//add the positions of neighboring units
		cohesionVect += neighbor->getPosition();
	}

	cohesionVect /= neighbors.size();

	return cohesionVect;
}