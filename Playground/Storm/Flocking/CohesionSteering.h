#pragma once

#include "Steering.h"
#include <vector>
class KinematicUnit;

class CohesionSteering : public Steering
{
public:
	CohesionSteering(KinematicUnit* pMover);
	~CohesionSteering();

	virtual Steering* getSteering();

	Vector2D getCohesion(const std::vector<KinematicUnit*>& neighbors);

private:
	KinematicUnit* mpMover;
};