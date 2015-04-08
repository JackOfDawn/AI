#include "Game.h"
#include "Kinematic.h"
#include "KinematicUnit.h"
#include "Sprite.h"
#include "GraphicsSystem.h"
#include "Steering.h"
#include "KinematicSeekSteering.h"
#include "KinematicArriveSteering.h"
#include "KinematicWanderSteering.h"
#include "DynamicSeekSteering.h"
#include "DynamicArriveSteering.h"
#include "WanderAndSeekSteering.h"

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


	if( steering->shouldApplyDirectly() )
	{
		//not stopped
		if( getVelocity().getLengthSquared() > MIN_VELOCITY_TO_TURN_SQUARED )
		{
			setVelocity( steering->getLinear() );
			setOrientation(steering->getAngular() );
			//setNewOrientation();
		}

		//since we are applying the steering directly we don't want any rotational velocity
		
		setRotationalVelocity( 0.0f );
		steering->setAngular( 0.0f );
	}
	else
	{
		setNewOrientation(); //updates the orientation of most types
	}
	
	//move the unit using current velocities
	Kinematic::update( time );
	//calculate new velocities
	calcNewVelocities( *steering, time, mMaxVelocity, 25.0f );
	//move to oposite side of screen if we are off
	GRAPHICS_SYSTEM->wrapCoordinates( mPosition );

	//set the orientation to match the direction of travel
	//setNewOrientation();
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

void KinematicUnit::seek(const Vector2D &target)
{
	KinematicSeekSteering* pSeekSteering = new KinematicSeekSteering( this, target );
	setSteering( pSeekSteering );
}

void KinematicUnit::arrive(const Vector2D &target)
{
	KinematicArriveSteering* pArriveSteering = new KinematicArriveSteering( this, target );
	setSteering( pArriveSteering );
}

void KinematicUnit::wander()
{
	KinematicWanderSteering* pWanderSteering = new KinematicWanderSteering( this );
	setSteering( pWanderSteering );
}

void KinematicUnit::dynamicSeek( KinematicUnit* pTarget )
{
	DynamicSeekSteering* pDynamicSeekSteering = new DynamicSeekSteering(this, pTarget);
	setSteering( pDynamicSeekSteering );
}

void KinematicUnit::dynamicFlee( KinematicUnit* pTarget )
{
	DynamicSeekSteering* pDynamicSeekSteering = new DynamicSeekSteering(this, pTarget, true);
	setSteering( pDynamicSeekSteering );
}

void KinematicUnit::dynamicArrive( KinematicUnit* pTarget )
{
	DynamicArriveSteering* pDynamicArriveSteering = new DynamicArriveSteering(this, pTarget);
	setSteering( pDynamicArriveSteering );
}


void KinematicUnit::dynamicWanderFlee(KinematicUnit* pTarget)
{
	WanderAndSeekSteering* pWanderAndFlee = new WanderAndSeekSteering(this, pTarget, true);
	setSteering(pWanderAndFlee);
}
void KinematicUnit::dynamicWanderSeek(KinematicUnit* pTarget)
{
	WanderAndSeekSteering* pWanderAndSeek = new WanderAndSeekSteering(this, pTarget, false);
	setSteering(pWanderAndSeek);
}


bool KinematicUnit::setDynamicSteering(SteeringType behavior, KinematicUnit* pTarget)
{
	switch (behavior)
	{
		case DYNAMIC_SEEK:
			this->dynamicSeek(pTarget);
			break;
		case DYNAMIC_FLEE:
			this->dynamicFlee(pTarget);
			break;
		case DYNAMIC_ARRIVE:
			this->dynamicArrive(pTarget);
			break;
		case WANDER:
			this->wander();
			break;
		case DYNAMIC_WANDER_FLEE:
			this->dynamicWanderFlee(pTarget);
			break;
		case DYNAMIC_WANDER_SEEK:
			this->dynamicWanderSeek(pTarget);
			break;
		default:
			return false;
			break;
	}
	return true;
}
