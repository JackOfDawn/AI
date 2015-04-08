#include "Rect2D.h"
#include <math.h>
#include <algorithm>

Rect2D::Rect2D(int x, int y, int width, int height)
{
	mUpperLeft = Vector2D(x, y);
	mUpperRight = Vector2D(x + width, y);
	mLowerLeft = Vector2D(x, y + height);
	mLowerRight = Vector2D(x + width, y + height);
	mWidth = width;
	mHeight = height;

}

Rect2D::Rect2D()
{
	mUpperLeft = Vector2D();
	mUpperRight = Vector2D();
	mLowerLeft = Vector2D();
	mLowerRight = Vector2D();
	mWidth = 0;
	mHeight = 0;

}

Rect2D::~Rect2D()
{

}


Vector2D Rect2D::getEdgeLine(Vector2D point)
{
	Vector2D edgeVector = Vector2D();

	float dTop, dBottom, x, y;

	dTop = abs(point.getY() - mUpperLeft.getY());
	dBottom = abs(point.getY() - mLowerRight.getY());

	//get y value
	if (dTop < dBottom) y = mUpperLeft.getY();
	else y = mLowerRight.getY();

	float dY = std::min(dBottom, dTop);

	//checksides
	float dLeft, dRight;
	dLeft = abs(point.getX() - mUpperLeft.getX());
	dRight = abs(point.getX() - mLowerRight.getX());

	//get x value
	if (dRight < dLeft) x = mLowerRight.getX();
	else x = mUpperLeft.getX();

	float dX = std::min(dLeft, dRight);

	Vector2D firstPoint = Vector2D(x, y);
	Vector2D secondPoint = firstPoint;
	//get a second point on the closest plane
	Vector2D distance = getDistanceVector(point);

	if (distance.getY() == 0)
	{
		if (firstPoint.getY() == mHeight)
		{
			secondPoint.setY(secondPoint.getY() - mHeight);
		}
		else
		{
			secondPoint.setY(secondPoint.getY() + mHeight);
		}
	}
	else
	{
		if (firstPoint.getX() == mWidth)
		{
			secondPoint.setX(secondPoint.getX() - mWidth);
		}
		else
		{
			secondPoint.setX(secondPoint.getX() + mWidth);
		}
	}
	//get the vector along the line
	edgeVector = secondPoint - firstPoint;

	

	return edgeVector;
}

Vector2D Rect2D::getClosestPoint(Vector2D point)
{
	return Vector2D();
}

Vector2D Rect2D::getDistanceVector(Vector2D point)
{
	float dX = std::max(abs(point.getX() - mUpperLeft.getX()) - mWidth, 0.0f);
	float dY = std::max(abs(point.getY() - mUpperLeft.getY()) - mHeight, 0.0f);

	Vector2D distanceVector = Vector2D(dX, dY);

	return distanceVector;
}

float Rect2D::getDistanceToEdge(Vector2D point)
{

	float dX = std::max(abs(point.getX() - mUpperLeft.getX()) - mWidth, 0.0f);
	float dY = std::max(abs(point.getY() - mUpperLeft.getY()) - mHeight, 0.0f);

	Vector2D distanceVector = Vector2D(dX, dY);

	return distanceVector.getLength();

	/* //this gets nearest corner...
	//check top and bottom
	float dTop, dBottom;
	
	dTop = abs(point.getY() - mUpperLeft.getY());
	dBottom = abs(point.getY() - mLowerRight.getY());

	float dY = std::min(dTop, dBottom);

	//checksides
	float dLeft, dRight;
	dLeft = abs(point.getX() - mUpperLeft.getX());
	dRight = abs(point.getX() - mLowerRight.getX());

	float dX = std::min(dLeft, dRight);

	

	//Make Vector
	Vector2D distanceVector = Vector2D(dX, dY);
	//find the magnitude
	float distance = distanceVector.getLength();
	//return the magnitude

	return distance;

	*/
}