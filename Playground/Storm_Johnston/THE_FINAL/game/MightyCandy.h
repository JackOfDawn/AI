#pragma once

#include "KinematicUnit.h"

class MightyCandy : public KinematicUnit
{
public:
	MightyCandy(Sprite* pSprite);
	~MightyCandy();

	void loadIn(SpawnInfo spawn);
	virtual void livingDraw(GraphicsBuffer* pBuffer);
};

