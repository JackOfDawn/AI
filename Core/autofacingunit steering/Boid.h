#pragma once

#include "AutoFacingUnit.h"

class Boid : public AutoFacingUnit
{
public:
	Boid(Sprite* pSprite, const Vector2D& position, float orientation, const Vector2D& velocity, float rotationVel, float maxVelocity = 1.0f, float maxAcceleration = 1.0f );
	virtual ~Boid();

	void update(float time);

	void setSteering( Steering* pSteering);
};