#pragma once

#include "Steering.h"
#include <vector>

class KinematicUnit;

class OrientationMatchingSteering : public Steering
{
public:
	OrientationMatchingSteering(KinematicUnit* pMover);
	~OrientationMatchingSteering(){};

	virtual Steering* getSteering();

	float matchOrientation(std::vector<KinematicUnit*>& neighbors);
private:
	KinematicUnit* mpMover;
};