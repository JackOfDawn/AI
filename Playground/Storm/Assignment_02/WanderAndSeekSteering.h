#pragma once

#include "Steering.h"
#include "Rect2D.h"
#include <vector>

class KinematicUnit;

const float MAX_DYNAMIC_WANDER_ROTATION = 1.0f; //maximum rate at which orientaion can change
const float WANDER_FORWARD_OFFSET = 2.0f;
const float WANDER_RADIUS = 10.0f;
const float MINIMUM_DISTANCE = 200.0f;


const float BOUNCE_RADIUS = 16.0f;

class WanderAndSeekSteering : public Steering
{
public:
	WanderAndSeekSteering(KinematicUnit* pMover, KinematicUnit *pTarget, bool shouldFlee);
	~WanderAndSeekSteering(){};

	void setTarget(KinematicUnit* pTarget) { mpTarget = pTarget; }

	virtual Steering* getSteering();

private:

	std::vector<Rect2D> checkWalls();
	void wander();
	void seek();
	void checkIfShouldWander();
	Vector2D asVector(float orientation);

	KinematicUnit* mpTarget;
	KinematicUnit* mpMover;
	bool mShouldFlee;
	bool mShouldWander;

	float mWanderOrientation;
};
