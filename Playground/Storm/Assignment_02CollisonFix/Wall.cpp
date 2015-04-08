#include "Wall.h"
#include <math.h>

Wall::Wall(Vector2D normal, Vector2D point)
:mNormal(normal)
, mPoint(point)
{
	mNormal.normalize();
}

float Wall::distanceFromPoint(const Vector2D& point)
{
	Vector2D pVector = point - mPoint;
	// abs(|w v|) / ||v||
	float distance = abs(dot(pVector, mNormal)) / mNormal.getLength();
	return distance;
}
bool Wall::checkLineCollision(const Vector2D& origin, const Vector2D& velocity)
{
	if (dot(velocity, mNormal) == 0) return false;
	// dot n(pl - po) / dot n v
	float t = dot(mNormal, mPoint - origin) / dot(mNormal, velocity);
	return t > 0 && t <= 1;
	//float p = dot(origin, mNormal) + distanceFromPoint(origin);
	//return p <= 0.0f;
}
Vector2D Wall::getCollisionPoint(const Vector2D& point, const Vector2D& direction)
{

	float t = dot(mNormal, mPoint - point) / dot(mNormal, direction);
	return point + direction * t;

}
Vector2D Wall::getResultantBouncePoint(const Vector2D& point, const Vector2D& direction)
{
	Vector2D resultant = point + getBounceVector(point, direction);
	return resultant;
}

Vector2D Wall::getBounceVector(const Vector2D& point, const Vector2D& direction)
{
	Vector2D collisionPoint = getCollisionPoint(point, direction);
	//pr = mp - 2projn(mp) 
	Vector2D pq = point + direction;
	Vector2D mp = collisionPoint - point;
	Vector2D pr = (mp - (proj(mNormal, mp) * 2));
	
	//pb = ||pq||/||pr|| * pr
	Vector2D pb = pr * (pq.getLength() / pr.getLength());
	return pb;
}


float dot(const Vector2D& vec1, const Vector2D& vec2)
{
	float x1 = vec1.getX(),
		x2 = vec2.getX(),
		y1 = vec1.getY(),
		y2 = vec2.getY(),
		dotProduct;

	dotProduct = (x1 * x2 + y1 * y2);
	return dotProduct;
}
Vector2D proj(const Vector2D& onto, const Vector2D& from)
{
	return onto * (dot(from, onto) / onto.getLengthSquared());
}