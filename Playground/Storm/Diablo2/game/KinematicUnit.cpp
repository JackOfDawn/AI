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


using namespace std;

Steering gNullSteering( gZeroVector2D, 0.0f );

KinematicUnit::KinematicUnit(Sprite *pSprite, const Vector2D &position, float orientation, const Vector2D &velocity, float rotationVel, float maxVelocity, float maxAcceleration)
:Kinematic( position, orientation, velocity, rotationVel )
,mpSprite(pSprite)
,mpCurrentSteering(NULL)
,mMaxVelocity(maxVelocity)
,mMaxAcceleration(maxAcceleration)
{
}

KinematicUnit::~KinematicUnit()
{
	delete mpCurrentSteering;
}

void KinematicUnit::draw( GraphicsBuffer* pBuffer )
{
	mpSprite->draw( *pBuffer, mPosition.getX(), mPosition.getY(), mOrientation );
}

void KinematicUnit::update(float time)
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
	Kinematic::update( time );
	//calculate new velocities
	calcNewVelocities( *steering, time, mMaxVelocity, 25.0f );

	
	if (Vector2D::distanceSq(mPosition, mPathTargets[mCurrentTargetIndex]) < 500)
	{
		if (mReverse)
		{
			mCurrentTargetIndex--;
			if (mCurrentTargetIndex == -1)
			{
				mCurrentTargetIndex = 1;
				mReverse = false;
				//Switch the MAP
				notifyPathEnd();
			}
			else
			{
				dynamic_cast<DynamicSeekSteering*>(mpCurrentSteering)->setTarget(mPathTargets[mCurrentTargetIndex]);
			}

		}
		else
		{
			mCurrentTargetIndex++;
			if (mPathTargets.size() == mCurrentTargetIndex)
			{
					mCurrentTargetIndex = mPathTargets.size()-2;
					mReverse = true;
					//switch map;
					notifyPathEnd();
			}
			else
			{
				dynamic_cast<DynamicSeekSteering*>(mpCurrentSteering)->setTarget(mPathTargets[mCurrentTargetIndex]);
				
			}
		}
	
	}
	
;
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


void KinematicUnit::dynamicSeek( std::vector<Vector2D> pTarget, bool Reverse )
{
	DynamicSeekSteering* pDynamicSeekSteering;
	if (pTarget.size() >= 2)
	{
		if (Reverse)
		{
			mCurrentTargetIndex = pTarget.size() - 1;
			pDynamicSeekSteering = new DynamicSeekSteering(this, pTarget[mCurrentTargetIndex -1 ]);
			this->mPosition = pTarget[mCurrentTargetIndex] - 1;
			this->setVelocity(pTarget[mCurrentTargetIndex] - mPosition);
			mReverse = Reverse;
		}
		else
		{
			mCurrentTargetIndex = 0;
			pDynamicSeekSteering = new DynamicSeekSteering(this, pTarget[mCurrentTargetIndex + 1]);
			this->mPosition = pTarget[mCurrentTargetIndex] - 1;
			this->setVelocity(pTarget[mCurrentTargetIndex] - mPosition);
			mReverse = Reverse;
		}
	}

	setSteering( pDynamicSeekSteering );
	mPathTargets = pTarget;
	
}


