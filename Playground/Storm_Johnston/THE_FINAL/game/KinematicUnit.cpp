#include "Game.h"
#include "Kinematic.h"
#include "KinematicUnit.h"
#include "Sprite.h"
#include "GraphicsSystem.h"
#include "Steering.h"
#include "DynamicSeekSteering.h"
#include "CheckEndPathMessage.h"
#include "GameMessage.h"
#include "GameApp.h"
#include "GameMessageManager.h"
#include <Grid.h>
#include <Map.h>
#include "GridPathfinder.h"
#include "GridGraph.h"
#include "MapHolder.h"
#include <algorithm>

using namespace std;

Steering gNullSteering( gZeroVector2D, 0.0f );
const int TARGET_LEEWAY = 20;

KinematicUnit::KinematicUnit(Sprite *pSprite, const Vector2D &position, float orientation, const Vector2D &velocity, float rotationVel, float maxVelocity, float maxAcceleration)
:Kinematic(position, orientation, velocity, rotationVel)
, mpSprite(pSprite)
, mpCurrentSteering(NULL)
, mpPathfinder(NULL)
, mMaxVelocity(maxVelocity)
, mMaxAcceleration(maxAcceleration)
, mpCurrentGridGraph(NULL)
, mpCurrentMap(NULL)
{
	mType = NULL_TYPE;
	mDead = false;
	mDeathTimer = 0.0f;
}

KinematicUnit::~KinematicUnit()
{
	delete mpCurrentSteering;
	delete mpPathfinder;
}

void KinematicUnit::draw( GraphicsBuffer* pBuffer )
{
	if (mDead)
	{
		deadDraw(pBuffer);
	}
	else
	{
		livingDraw(pBuffer);
	}
}

void KinematicUnit::livingDraw(GraphicsBuffer* pBuffer)
{
	mpSprite->draw( *pBuffer, mPosition.getX(), mPosition.getY(), mOrientation, 1 );
}

void KinematicUnit::deadDraw(GraphicsBuffer* pBuffer)
{
}

void KinematicUnit::update(float time)
{
	if (mDead)
	{
		deadUpdate(time);
	}
	else
	{
		livingUpdate(time);
	}
}

void KinematicUnit::livingUpdate(float time)
{
	Steering* steering;
	if( mpCurrentSteering != NULL )
	{
		steering = mpCurrentSteering->getSteering();
	}
	else
	{
		steering = &gNullSteering;
	}
	
	setNewOrientation();

	//move the unit using current velocities
	Kinematic::update(time);
	//calculate new velocities
	calcNewVelocities(*steering, time, mMaxVelocity, 25.0f);
	if (steering != &gNullSteering)
	{
		if (mPathTargets.size() > (unsigned int)mCurrentTargetIndex && Vector2D::distanceSq(mPosition, mPathTargets[mCurrentTargetIndex]) < TARGET_LEEWAY * TARGET_LEEWAY)
		{
			mCurrentTargetIndex++;
			if (mPathTargets.size() == mCurrentTargetIndex)
			{
				mPathingInfo.currentPath++; 
				if (mPathingInfo.currentPath == mPathingInfo.mapIDs.size()) //if the reaxhes the real end of the path
				{
					mPathingInfo = PathingInfo(); // resets pathing info;
				}
				else //reached the end of the current path in current map
				{
					mCurrentMapID = mPathingInfo.mapIDs[mPathingInfo.currentPath];
					updateCurrentMap();
				}
			}
			else
			{
				dynamic_cast<DynamicSeekSteering*>(mpCurrentSteering)->setTarget(mPathTargets[mCurrentTargetIndex]);
			}			
		}
	}
}

void KinematicUnit::deadUpdate(float time)
{
	mDeathTimer -= 0.066f;

	if (mDeathTimer <= 0.0f)
	{
		respawn();
	}
}

void KinematicUnit::updateCurrentMap()
{
	mpCurrentMap = gpMapHolder->getMap(mCurrentMapID);
	mpCurrentGridGraph = gpMapHolder->getGridGraph(mCurrentMapID);
	mpPathfinder->changeGraph(mpCurrentGridGraph);
	mPosition = mpCurrentGridGraph->getLocation(mPathingInfo.nodePairs[mPathingInfo.currentPath].first) + mpCurrentGridGraph->getSquareSize() / 2;
	mNeedsNewPath = true;
}

void KinematicUnit::notifyPathEnd()
{
	GameMessage* pMessage = new CheckPathEndMessage();
	dynamic_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
}

//private - deletes old Steering before setting
void KinematicUnit::setSteering( Steering* pSteering )
{
	delete mpCurrentSteering;
	mpCurrentSteering = pSteering;
}

void KinematicUnit::setNewOrientation()
{ 
	mOrientation = getOrientationFromVelocity( mOrientation, mVelocity ); 
}

void KinematicUnit::dynamicSeek()
{
	DynamicSeekSteering* pDynamicSeekSteering = new DynamicSeekSteering(this, this->mPosition + Vector2D(1, 1));

	setSteering( pDynamicSeekSteering );	
}


void KinematicUnit::visualizePath(Grid* pGrid, GraphicsBuffer* pBackBuffer)
{
	if (mpPathfinder != NULL)
		mpPathfinder->drawVisualization(pGrid, pBackBuffer);
}


void KinematicUnit::findNewPath()
{
	int* currentPath = &mPathingInfo.currentPath;
	mpPathfinder->findPath(mPathingInfo.nodePairs.at(*currentPath).first, mPathingInfo.nodePairs.at(*currentPath).second);
	mPathTargets = mpCurrentGridGraph->convertPathNodesToVectors(mpPathfinder->returnPath());
	mCurrentTargetIndex = 0;
	mNeedsNewPath = false;
}

void KinematicUnit::generatePathing(EndPoint& endPoint)
{
	EndPoint startTarget;
	Grid* grid = mpCurrentMap->getGrid();
	startTarget.index = grid->getSquareIndexFromPixelXY((int)mPosition.getX(), (int)mPosition.getY());
	startTarget.mapID = mCurrentMapID;

	mPathingInfo = gpMapHolder->generatePath(startTarget, endPoint);
	mNeedsNewPath = true;
}

int KinematicUnit::getIndexOnGraph()
{
	return mpCurrentMap->getGrid()->getSquareIndexFromPixelXY(mPosition.getX(), mPosition.getY());
}

bool KinematicUnit::dieTill(float tillRespawn)
{
	mDeathTimer = tillRespawn;
	return mDead = true;
}

void KinematicUnit::respawn()
{
	mDead = false;
	mpCurrentMap = gpMapHolder->getMap(mSpawnInfo.mapID);
	mCurrentMapID = mSpawnInfo.mapID;
	GridGraph* tmp = gpMapHolder->getGridGraph(mSpawnInfo.mapID);
	mPosition = tmp->getLocation(tmp->getNode(mSpawnInfo.location)) + tmp->getSquareSize()/2;
	mpCurrentGridGraph = tmp;
	mpPathfinder->changeGraph(tmp);
	resetPathingInfo();
}

float KinematicUnit::getSizeScaled()
{
	return ((mpSprite->getWidth() + mpSprite->getHeight()) / 2) * getScale();
}

float KinematicUnit::getScale()
{
	return min(mpCurrentMap->getScale() * 0.80f, 1.0f);
}