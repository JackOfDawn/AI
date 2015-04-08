#include "Player.h"
#include "MapHolder.h"
#include "AStarPathFinder.h"
#include "GridGraph.h"
#include <Sprite.h>
#include <Grid.h>


Player::Player(Sprite* pSprite, SpawnInfo spawnInfo)
:KinematicUnit(pSprite, Vector2D(), 0.01f, Vector2D(), 0.0f, 100.0f, 2000.0f)
{
	mSpawnInfo = spawnInfo;
	mType = PLAYER;
	mInvincibility = 0;
	mEatenTime = 0;
	mGodMode = false;
}

Player::~Player()
{
}

void Player::LoadIn()
{
	mpCurrentMap = gpMapHolder->getMap(mSpawnInfo.mapID);
	mCurrentMapID = mSpawnInfo.mapID;
	GridGraph* tmp = gpMapHolder->getGridGraph(mSpawnInfo.mapID);
	mpPathfinder = new AStarPathfinder(tmp);
	mPosition = tmp->getLocation(tmp->getNode(mSpawnInfo.location)) + tmp->getSquareSize()/2;
	mpCurrentGridGraph = tmp;
}

void Player::livingUpdate(float time)
{
	if (mPathingInfo.isValid())
	{
		if (mNeedsNewPath)
		{
			findNewPath();
		}
		KinematicUnit::livingUpdate(time);
	}
	
	if (!mGodMode)
		mInvincibility -= 0.066f;
	else
		mInvincibility = 5.0f;

	mEatenTime -= 0.05f;
}

void Player::livingDraw(GraphicsBuffer* pBuffer)
{
	float scale = getScale() * max(mEatenTime / (INVINCIBLE_TIME * 0.80), 1);

	if (mInvincibility >= 0.0f)
	{
		if (mInvincibility <= 3.0f)
		{
			if (mBlinkTime >= 5.0f)
			{
				mpSprite->draw(*pBuffer, mPosition.getX(), mPosition.getY(), mOrientation, scale, mPowerColor);
				mBlinkTime -= 1.0f;
			}
			else
			{
				mpSprite->draw(*pBuffer, mPosition.getX(), mPosition.getY(), mOrientation, scale);
				mBlinkTime -= 3.0f;
			}
			if (mBlinkTime <= 0.0f)
			{
				mBlinkTime = 10.0f;
			}
		}
		else
		{
			mpSprite->draw(*pBuffer, mPosition.getX(), mPosition.getY(), mOrientation, scale, mPowerColor);
		}
	}
	else
	{
		mpSprite->draw(*pBuffer, mPosition.getX(), mPosition.getY(), mOrientation, scale);
	}

}

//void Player::generatePathing(EndPoint endPoint)
//{
//	EndPoint startTarget;
//	Grid* grid = mpCurrentMap->getGrid();
//	startTarget.index = grid->getSquareIndexFromPixelXY((int)mPosition.getX(), (int)mPosition.getY());
//	startTarget.mapID = mCurrentMapID;
//
//	mPathingInfo = gpMapHolder->generatePath(startTarget, endPoint);
//	mNeedsNewPath = true;
//}