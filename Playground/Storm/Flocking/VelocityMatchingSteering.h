#pragma once

#include "Steering.h"
#include <vector>

class KinematicUnit;

class VelocityMatchingSteering : public Steering
{
public:
	VelocityMatchingSteering(KinematicUnit* pMover);
	~VelocityMatchingSteering(){};

	virtual Steering* getSteering();

	Vector2D matchVelocity(const std::vector<KinematicUnit*>& neighbors);
private:
	KinematicUnit* mpMover;

};
