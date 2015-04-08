#include "WeightedBlendingSteering.h"

WeightedBlendingSteering::WeightedBlendingSteering(vector<Steering*> steerings, vector<float> weights, float maxAcc)
	: Steering(gZeroVector2D)
{
	mSteerings = steerings;
	mWeights = weights;
	mMaxAcceleration = maxAcc;
}

WeightedBlendingSteering::~WeightedBlendingSteering()
{
	for (int i = 0; i < mSteerings.size(); i++)
	{
		delete mSteerings[i];
	}

	mSteerings.clear();
}

Steering* WeightedBlendingSteering::getSteering()
{
	mLinear = gZeroVector2D;
	float totalWeight = 0.0f;

	// go through all the steerings and add them together and multiply by the weights
	for (int i = 0; i < mSteerings.size(); i++)
	{
		mLinear += mSteerings[i]->getSteering()->getLinear() * mWeights[i];
		totalWeight += mWeights[i];
	}

	mLinear /= totalWeight;
	
	// if the velocity is higher than our max acceleration set it to the max acceleration
	if (mLinear.getLength() > mMaxAcceleration)
	{
		mLinear.normalize();
		mLinear *= mMaxAcceleration;
	}

	

	return this;
}