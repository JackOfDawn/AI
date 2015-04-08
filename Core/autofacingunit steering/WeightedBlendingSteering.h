#pragma once

#include "Steering.h"
#include <vector>
using namespace std;

class WeightedBlendingSteering : public Steering
{
public:
	WeightedBlendingSteering(vector<Steering*> steerings, vector<float> weights, float maxAcc);
	~WeightedBlendingSteering();

	Steering* getSteering();

private:
	vector<Steering*> mSteerings;
	vector<float> mWeights;
	float mMaxAcceleration;
};