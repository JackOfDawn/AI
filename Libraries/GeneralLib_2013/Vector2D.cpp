#include "Vector2D.h"
#include <cmath>

Vector2D gZeroVector2D( 0.0f, 0.0f );

Vector2D::Vector2D(float x, float y)
:mX(x)
,mY(y)
{
}

Vector2D::Vector2D( const Vector2D& rhs )
:mX( rhs.mX )
,mY( rhs.mY )
{
}

Vector2D::~Vector2D()
{
}

Vector2D& Vector2D::operator += ( const Vector2D& rhs )
{
	mX += rhs.mX;
	mY += rhs.mY;
	return *this;
}

Vector2D& Vector2D::operator += (float rhs)
{
	mX += rhs;
	mY += rhs;
	return *this;
}

Vector2D& Vector2D::operator -= ( const Vector2D& rhs )
{
	mX -= rhs.mX;
	mY -= rhs.mY;
	return *this;
}

Vector2D& Vector2D::operator -= (float rhs)
{
	mX -= rhs;
	mY -= rhs;
	return *this;
}

Vector2D& Vector2D::operator *= (const Vector2D& rhs)
{
	mX *= rhs.mX;
	mY *= rhs.mY;
	return *this;
}

Vector2D& Vector2D::operator *= ( float rhs )
{
	mX *= rhs;
	mY *= rhs;
	return *this;
}

Vector2D& Vector2D::operator /= (const Vector2D& rhs)
{
	mX /= rhs.mX;
	mY /= rhs.mY;
	return *this;
}

Vector2D& Vector2D::operator /= ( float rhs )
{
	mX /= rhs;
	mY /= rhs;
	return *this;
}

Vector2D& Vector2D::operator = (const Vector2D& rhs)
{
	mX = rhs.mX;
	mY = rhs.mY;
	return *this;
}

Vector2D& Vector2D::operator = (float rhs)
{
	mX = rhs;
	mY = rhs;
	return *this;
}

const Vector2D Vector2D::operator + (const Vector2D& rhs) const
{
	Vector2D result = *this;
	result += rhs;
	return result;
}

const Vector2D Vector2D::operator + (float rhs) const
{
	Vector2D result = *this;
	result += rhs;
	return result;
}

const Vector2D Vector2D::operator - (const Vector2D& rhs) const
{
	Vector2D result = *this;
	result -= rhs;
	return result;
}

const Vector2D Vector2D::operator - (float rhs) const
{
	Vector2D result = *this;
	result -= rhs;
	return result;
}

const Vector2D Vector2D::operator * (const Vector2D& rhs) const
{
	Vector2D result = *this;
	result *= rhs;
	return result;
}

const Vector2D Vector2D::operator * (float rhs) const
{
	Vector2D result = *this;
	result *= rhs;
	return result;
}

const Vector2D Vector2D::operator / (const Vector2D& rhs) const
{
	Vector2D result = *this;
	result /= rhs;
	return result;
}

const Vector2D Vector2D::operator / (float rhs) const
{
	Vector2D result = *this;
	result /= rhs;
	return result;
}

bool Vector2D::operator < (const Vector2D& rhs) const
{
	return (mX < rhs.getX()) || (mX == rhs.getX()) && (mY < rhs.getY());
}

bool Vector2D::operator > (const Vector2D& rhs) const
{
	return !(*this < rhs) && !(*this == rhs);
}

bool Vector2D::operator <= (const Vector2D& rhs) const
{
	return (*this == rhs) || (*this < rhs);
}

bool Vector2D::operator >= (const Vector2D& rhs) const
{
	return (*this == rhs) || (*this > rhs);
}

bool Vector2D::operator == (const Vector2D& rhs) const
{
	return (mX == rhs.getX()) && (mY == rhs.getY());
}

bool Vector2D::operator != (const Vector2D& rhs) const
{
	return !(*this == rhs);
}

bool Vector2D::hasNonZeroLength() const
{
	if( mX != 0.0f || mY != 0.0f )
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Vector2D::getLength() const
{
	float lengthSquared = getLengthSquared();
	return sqrt( lengthSquared );
}

float Vector2D::getLengthSquared() const
{
	float lengthSquared = ( mX * mX ) + ( mY * mY );
	return lengthSquared;
}

void Vector2D::normalize(float length /* = 1 */)
{
	float vLength = getLength();
	mX /= vLength;
	mY /= vLength;

	mX *= length;
	mY *= length;
}

// Static Functions

float Vector2D::dot(const Vector2D& v1, const Vector2D& v2)
{
	return ((v1.getX() * v2.getX()) + (v1.getY() * v2.getY()));
}

float Vector2D::distanceSq(const Vector2D& v1, const Vector2D& v2)
{
	return ((v1.getX() - v2.getX()) * (v1.getX() - v2.getX())) + ((v1.getY() - v2.getY()) * (v1.getY() - v2.getY()));
}

float Vector2D::distance(const Vector2D& v1, const Vector2D& v2)
{
	return sqrt(distanceSq(v1, v2));
}

const Vector2D Vector2D::min(const Vector2D& v1, const Vector2D& v2)
{
	return Vector2D((v1.getX() < v2.getX()) ? v1.getX() : v2.getX(), (v1.getY() < v2.getY()) ? v1.getY() : v2.getY());
}

const Vector2D Vector2D::max(const Vector2D& v1, const Vector2D& v2)
{
	return Vector2D((v1.getX() > v2.getX()) ? v1.getX() : v2.getX(), (v1.getY() > v2.getY()) ? v1.getY() : v2.getY());
}

const Vector2D Vector2D::clamp(const Vector2D& vec, const Vector2D& minVec, const Vector2D& maxVec)
{
	return min(max(vec, minVec), maxVec);
}

const Vector2D Vector2D::Project(const Vector2D& u, const Vector2D& v)
{
	return v * ((dot(u, v)) / (dot(v, v)));
}