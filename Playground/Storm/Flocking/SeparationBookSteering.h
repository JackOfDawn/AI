#pragma once

#include "Steering.h"
#include <vector>
class KinematicUnit;

const float THRESHOLD = 100;


class SeparationBookSteering : public Steering
{
public:
	SeparationBookSteering(KinematicUnit* pMover);
	~SeparationBookSteering(){};

	virtual Steering* getSteering();

private:
	KinematicUnit* mpMover;
};