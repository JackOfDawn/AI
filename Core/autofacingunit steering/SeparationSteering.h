#include "Steering.h"
#include "Kinematic.h"
#include "KinematicUnit.h"

class SeparationSteering : public Steering
{
public:
	SeparationSteering(KinematicUnit* mover, int radius, int decay, int maxAcc);
	~SeparationSteering();

	Steering* getSteering();

private:
	KinematicUnit* mpMover;
	int mRadius;
	float mDecay;
	int mMaxAcceleration;
};