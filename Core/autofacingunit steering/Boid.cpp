#include "Boid.h"
#include "Game.h"

Boid::Boid(Sprite* pSprite, const Vector2D& position, float orientation, const Vector2D& velocity, float rotationVel, float maxVelocity, float maxAcceleration)
	: AutoFacingUnit(pSprite, position, orientation, velocity, rotationVel, maxVelocity, maxAcceleration)
{
}

Boid::~Boid()
{
}

void Boid::update(float time)
{
	AutoFacingUnit::update(time);
}

void Boid::setSteering(Steering* pSteering)
{
	mpCurrentSteering = pSteering;
}