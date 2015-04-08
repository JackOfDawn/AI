#include "Steering.h"

class KinematicUnit;

const float MAX_DYNAMIC_WANDER_ROTATION = 1.0f; //maximum rate at which orientaion can change
const float WANDER_FORWARD_OFFSET = 2.0f;
const float WANDER_RADIUS = 10.0f;

class DynamicWanderSteering : public Steering
{
public:
	DynamicWanderSteering(KinematicUnit* pMover);
	~DynamicWanderSteering(){}

	virtual Steering* getSteering();

	Vector2D asVector(float orientation);

private:
	float mWanderOrientation;
	KinematicUnit* mpMover;
};