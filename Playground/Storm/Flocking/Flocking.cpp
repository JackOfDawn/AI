#include "Flocking.h"
#include "KinematicUnit.h"
#include "CohesionSteering.h"
#include "SeparationSteering.h"
#include "DynamicSeekSteering.h"
#include "DynamicWanderSteering.h"
#include "Defines.h"
#include "Game.h"
#include "UnitManager.h"

const float MAGNITUDE = 10;

float FlockingSteering::mAlignWeight = .1f;
float FlockingSteering::mCohesionWeight = .0f;
float FlockingSteering::mSeparationWeight = .2f;


FlockingSteering::FlockingSteering(KinematicUnit* pMover, KinematicUnit* pTarget)
:mpMover(pMover)
{
	mCohesionSteering = new CohesionSteering(mpMover);
	mAlignmentSteering = new GroupAlignmentSteering(mpMover);
	//mSeparationSteering = new SeparationSteering(mpMover);
	mSeparationSteering = new SeparationBookSteering(mpMover);
	//mSeek = new DynamicSeekSteering(mpMover, pTarget);
	mSeek = new DynamicWanderSteering(mpMover);
	mApplyDirectly = false;
	mAlignment = mCohesion = mSeparation = Vector2D();
}

FlockingSteering::~FlockingSteering()
{
	delete mAlignmentSteering;
	delete mSeparationSteering;
	delete mCohesionSteering;
	delete mSeek;
}

Steering* FlockingSteering::getSteering()
{
	
	std::vector<KinematicUnit*> neighbors = mpMover->getNeighbors();

	mSeek->getSteering();

	mLinear = mSeek->getLinear();
	mAngular = mSeek->getAngular();

	if (neighbors.size() != 0)
	{
		mCohesionSteering->getSteering();
		mSeparationSteering->getSteering();
		mAlignmentSteering->getSteering();
		mAlignment = mAlignmentSteering->getLinear() * (mAlignWeight);
		mSeparation = mSeparationSteering->getLinear() * (mSeparationWeight * MAGNITUDE);
		mCohesion = mCohesionSteering->getLinear() * (mCohesionWeight * MAGNITUDE);
		mLinear += mCohesion + mAlignment + mSeparation;

		mAngular = mAlignmentSteering->getAngular() * mAlignWeight; // weight this
	}

	return this;
}

void FlockingSteering::adjustWeight(WeightType type, float amt)
{
	switch (type)
	{
	case ALIGNMENT:
		mAlignWeight += amt;
		break;
	case COHESION :
		mCohesionWeight += amt;
		break;
	case SEPARATION:
		mSeparationWeight += amt;
	default:
		break;
	}

	if (mAlignWeight <= 0) mAlignWeight = 0;
	if (mCohesionWeight <= 0) mCohesionWeight = 0;
	if (mSeparationWeight <= 0) mSeparationWeight = 0;
}

float FlockingSteering::getWeight(WeightType type)
{
	float weight = -1;
	switch (type)
	{
	case ALIGNMENT:
		weight = mAlignWeight;
		break;
	case COHESION:
		weight = mCohesionWeight;
		break;
	case SEPARATION:
		weight = mSeparationWeight;
	default:
		break;
	}
	return weight;
}

void FlockingSteering::setWeight(WeightType type, float amt)
{
	switch (type)
	{
	case ALIGNMENT:
		mAlignWeight = amt;
		break;
	case COHESION:
		mCohesionWeight = amt;
		break;
	case SEPARATION:
		mSeparationWeight = amt;
	default:
		break;
	}
}