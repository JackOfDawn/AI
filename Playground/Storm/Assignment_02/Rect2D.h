#pragma once
#include "Vector2D.h"
class Rect2D
{
public:
	Rect2D(int x, int y, int width, int height);
	Rect2D();
	~Rect2D();

	float getDistanceToEdge(Vector2D point);
	Vector2D getClosestPoint(Vector2D point);
	Vector2D getDistanceVector(Vector2D point);
	Vector2D getEdgeLine(Vector2D point);

private:

	//upper left
	Vector2D mUpperLeft;
	Vector2D mUpperRight;
	Vector2D mLowerLeft;
	Vector2D mLowerRight;

	int mWidth;
	int mHeight;
};

