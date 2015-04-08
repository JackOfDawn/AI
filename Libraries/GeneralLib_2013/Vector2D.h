#pragma once

/* Vector2D is a generally usable 2D vector.  Most of the operator overloading code is patterned after the notes from 
	California Institue of Technology site: http://www.cs.caltech.edu/courses/cs11/material/cpp/donnie/cpp-ops.html .  
	Exact author unknown.
	May be missing some important functions but this should be enough to do most things.

	by Dean Lawson
	Champlain College
	2011
*/

#include "Trackable.h"
#include <cfloat>

#undef min
#undef max

class Vector2D : public Trackable
{
public:
	Vector2D( float x = 0.0f, float y = 0.0f );//constructor
	Vector2D( const Vector2D& rhs );//copy constructor
	~Vector2D();//destructor

	//math operators
	Vector2D& operator += (const Vector2D& rhs );
	Vector2D& operator += (float rhs);
	Vector2D& operator -= (const Vector2D& rhs);
	Vector2D& operator -= (float rhs);
	Vector2D& operator *= (const Vector2D& rhs);
	Vector2D& operator *= (float rhs);
	Vector2D& operator /= (const Vector2D& rhs);
	Vector2D& operator /= (float rhs);
	Vector2D& operator = (const Vector2D& rhs);
	Vector2D& operator = (float rhs);

	const Vector2D operator + (const Vector2D& rhs) const;
	const Vector2D operator + (float rhs) const;
	const Vector2D operator - (const Vector2D& rhs) const;
	const Vector2D operator - (float rhs) const;
	const Vector2D operator * (const Vector2D& rhs) const;
	const Vector2D operator * (float rhs) const;
	const Vector2D operator / (const Vector2D& rhs) const;
	const Vector2D operator / (float rhs) const;

	bool operator < (const Vector2D& rhs) const;
	bool operator > (const Vector2D& rhs) const;
	bool operator <= (const Vector2D& rhs) const;
	bool operator >= (const Vector2D& rhs) const;
	bool operator == (const Vector2D& rhs) const;
	bool operator != (const Vector2D& rhs) const;

	//getters and setters
	inline float getX() const { return mX; };
	inline float getY() const { return mY; };
	inline void setX( float x ) { mX = x; };
	inline void setY( float y ) { mY = y; };
	inline void setXY(float x, float y) { mX = x; mY = y; }
	
	// Static functions
	// The credit for these functions goes to Amble Lighthertz here http://codereview.stackexchange.com/questions/11934/c-vector2-class-review
	static float dot(const Vector2D& v1, const Vector2D& v2);
	static float distanceSq(const Vector2D& v1, const Vector2D& v2);
	static float distance(const Vector2D& v1, const Vector2D& v2);
	static const Vector2D min(const Vector2D& v1, const Vector2D& v2);
	static const Vector2D max(const Vector2D& v1, const Vector2D& v2);
	static const Vector2D clamp(const Vector2D& vec, const Vector2D& minVec, const Vector2D& maxVec);

	static const Vector2D Project(const Vector2D& u, const Vector2D& v);

	//length functions
	bool hasNonZeroLength() const;
	float getLength() const;
	float getLengthSquared() const; //more efficient than get length - use when comparing distances and actual distance is not needed

	void normalize(float length = 1); // Normalizes to a unit vector and multiplies by length passed (1 keeps it a unit vector)

private:
	float mX;
	float mY;
};

extern Vector2D gZeroVector2D;//useful when we need to return a pointer to a default vector from a function
