#pragma once

#include "Kinematic.h"
#include <vector>
class Sprite;
class GraphicsBuffer;

class Asteroid : public Kinematic
{
public:
	Asteroid(Sprite* pSprite, const Vector2D& position, float orientation, const Vector2D &velocity, float rotationVel, float maxVelocity = 1.0f, float maxAcceleration = 1.0f, float radius = 16);
	~Asteroid(){};

	const Vector2D& getPosition() const { return mPosition; };
	float getMaxVelocity() const { return mMaxVelocity; };
	Vector2D getVelocity() const { return mVelocity; };
	float getMaxAcceleration() const { return mMaxAcceleration; };
	void setVelocity(const Vector2D& velocity){ mVelocity = velocity; };
	float getRadius() const { return mRadius; }

	void handleCylinderCollisions();

	void draw(GraphicsBuffer* pBuffer);
	void update(float time);
	
private:
	Sprite* mpSprite;
	float mMaxVelocity;
	float mMaxAcceleration;
	float mRadius;
};