#pragma once

#include "KinematicUnit.h"
#include "GridPathfinder.h"
#include <Map.h>
#include <PortalInformation.h>
#include "MapDefines.h"

#include "PathingInfo.h"

enum AiStates
{
	PATROL_STATE = 0,
	FLEE_STATE,
	CHASE_STATE,
	NUM_STATES
};

class GrueState;

class Grue : public KinematicUnit
{
public:
	Grue(Sprite* pSprite);
	~Grue();

	int getValue(){ return mValue; }

	void LoadIn(SpawnInfo spawn, KinematicUnit* Player);
	void Spawn();
	virtual void livingUpdate(float time);
	virtual void livingDraw(GraphicsBuffer* pBuffer);

	inline void setState(GrueState* state) { mCurrentState = state; };

	void setValidPathingInfo(EndPoint& endpoint);

private:
	KinematicUnit* mpPlayer;
	
	GrueState* mCurrentState;
	EndPoint mPatrolTarget;

	int mValue;
};