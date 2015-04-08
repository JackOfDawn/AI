#pragma once

#include "Steering.h"
#include <vector>

class KinematicUnit;

class SeparationSteering : public Steering
{
public:
	SeparationSteering(KinematicUnit* pMover);
	~SeparationSteering();

	virtual Steering* getSteering();
	Vector2D getSeperationVector(const std::vector<KinematicUnit*>& neighbors);

private:
	KinematicUnit* mpMover;
};