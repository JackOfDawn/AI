#pragma once

#include "KinematicUnit.h"
#include "Steering.h"

class VelocityMatching : public Steering
{
public:
	VelocityMatching(KinematicUnit* mover, Vector2D targetVelocity, float timeToTarget, float maxAcceleration);
	~VelocityMatching();

	Steering* getSteering();

private:
	KinematicUnit* mpMover;
	Vector2D mTargetVelocity;
	float mTimeToTarget;
	float mMaxAcceleration;
};