#pragma once
/*Thanks to scott barrett for the wall header*/

#include <Vector2D.h>


class Wall
{
public:
	Wall(Vector2D normal, Vector2D point);

	Vector2D getNormal() { return mNormal; }
	Vector2D getAPoint() { return mPoint; }
	float distanceFromPoint(const Vector2D& point);
	bool checkLineCollision(const Vector2D& origin, const Vector2D& velocity);
	Vector2D getCollisionPoint(const Vector2D& point, const Vector2D& direction);
	Vector2D getResultantBouncePoint(const Vector2D& point, const Vector2D& direction);
	Vector2D getBounceVector(const Vector2D& point, const Vector2D& direction);

	float getDistance(const Vector2D& point);
	Vector2D getWallForce(const Vector2D& point, float acceleration);

private:
	Vector2D mNormal;
	Vector2D mPoint;
};

float dot(const Vector2D& vec1, const Vector2D& vec2);
Vector2D proj(const Vector2D& onto, const Vector2D& from);