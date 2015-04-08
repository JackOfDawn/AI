#pragma once

#include "KinematicUnit.h"
#include "Vector2D.h"

class Coin : public KinematicUnit
{
public:
	Coin(Vector2D location, Sprite* pSprite);
	~Coin();

	int getValue(){ return mValue; }

	virtual void livingDraw(GraphicsBuffer* pBuffer);

private:
	int mValue;
};

