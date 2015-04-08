#pragma once
#include "KinematicUnit.h"
#include "GridPathfinder.h"
#include <Map.h>
#include <PortalInformation.h>
#include "MapDefines.h"
#include "PathingInfo.h"

class Player : public KinematicUnit
{
public:
	Player(Sprite* pSprite, SpawnInfo spawnInfo);
	~Player();

	void resetInvincibility(){ mInvincibility = INVINCIBLE_TIME; }
	float getInvincibility(){ return mInvincibility; }
	void godMode(){ mGodMode = !mGodMode; mInvincibility = 0.0f; }
	int* getGodRef(){ return &mGodMode; }

	void LoadIn();
	virtual void livingUpdate(float time);
	virtual void livingDraw(GraphicsBuffer* pBuffer);
	
	void eat() {mEatenTime = INVINCIBLE_TIME; }

private:
	float mInvincibility;
	float mEatenTime;
	int mGodMode;

	const float INVINCIBLE_TIME = 10;
	float mBlinkTime = 10;

	const ALLEGRO_COLOR mPowerColor = al_map_rgba_f(1, 0, 0, 0.5);
};