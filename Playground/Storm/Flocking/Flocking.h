#pragma once

#include "Steering.h"
#include "CohesionSteering.h"
#include "SeparationSteering.h"
#include "SeparationBookSteering.h"
#include "GroupAlignmentSteering.h"
#include "Defines.h"

class KinematicUnit;



class FlockingSteering : public Steering
{
public:
	FlockingSteering(KinematicUnit* pMover, KinematicUnit* pTarget);
	~FlockingSteering();

	virtual Steering* getSteering();

	static void adjustWeight(WeightType type, float amt);
	static float getWeight(WeightType type);
	static void setWeight(WeightType type, float amt);


private:
	GroupAlignmentSteering* mAlignmentSteering;
	Steering* mSeparationSteering;
	CohesionSteering* mCohesionSteering;
	Steering* mSeek;

	Vector2D mAlignment,
		mSeparation,
		mCohesion;

	KinematicUnit* mpMover;

	static float mAlignWeight;
	static float mCohesionWeight;
	static float mSeparationWeight;
};