#include "Game.h"
#include "Kinematic.h"
#include "KinematicUnit.h"
#include "Sprite.h"
#include "GraphicsSystem.h"
#include "Steering.h"
#include "KinematicSeekSteering.h"
#include "KinematicArriveSteering.h"
#include "DynamicSeekSteering.h"
#include "DynamicArriveSteering.h"
#include "DynamicWanderSteering.h"
#include "UnitManager.h"
#include "Defines.h"
#include "Flocking.h"

using namespace std;

Steering gNullSteering( gZeroVector2D, 0.0f );

KinematicUnit::KinematicUnit(Sprite *pSprite, const Vector2D &position, float orientation, const Vector2D &velocity, float rotationVel, float maxVelocity, float maxAcceleration, float radius)
:Kinematic( position, orientation, velocity, rotationVel )
,mpSprite(pSprite)
,mpCurrentSteering(NULL)
,mMaxVelocity(maxVelocity)
,mMaxAcceleration(maxAcceleration)
, mRadius(radius)
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
	mNeighbors = UNIT_MANAGER->getNeighbors(this->getID());


	if( mpCurrentSteering != NULL )
	{
		steering = mpCurrentSteering->getSteering();
	}
	else
	{
		steering = &gNullSteering;
	}

	
	//check neighbor collisions
	handleCylinderCollisions();

	
	//Check wall collisions
	

	Vector2D wallForce = handleWallAvoidance();
	steering->setLinear(steering->getLinear() + wallForce);
	handleWallCollisions(time);
	
	//updates the orientation of most types
	if (steering->shouldApplyDirectly())
	{
		//not stopped
		if (getVelocity().getLengthSquared() > MIN_VELOCITY_TO_TURN_SQUARED)
		{
			setVelocity(steering->getLinear());
			setOrientation(steering->getAngular());
		}

		//since we are applying the steering directly we don't want any rotational velocity
		setRotationalVelocity(0.0f);
		steering->setAngular(0.0f);
	}

	
	//move the unit using current velocities
	Kinematic::update( time );
	//calculate new velocities
	
	calcNewVelocities( *steering, time, mMaxVelocity, 100.0f );
	//move to oposite side of screen if we are off
	GRAPHICS_SYSTEM->wrapCoordinates( mPosition );

	//set the orientation to match the direction of travel
	setNewOrientation();
}

void KinematicUnit::handleCylinderCollisions()
{
	std::vector<Asteroid*> asteroids = UNIT_MANAGER->getAsteroids();
	for (size_t i = 0; i < asteroids.size(); i++)
	{
		//check collisions and adjust
		Asteroid* unit2 = asteroids[i];

		Vector2D vectorToCenter = unit2->getPosition() - this->getPosition();

		Vector2D normalizedVelocity = this->getVelocity();
		normalizedVelocity.normalize();

		Vector2D projectedVector = proj(normalizedVelocity, vectorToCenter);
		Vector2D collisionPoint = this->getPosition() + projectedVector;


		Vector2D oc = collisionPoint - unit2->getPosition();

		float ocLength = oc.getLength();

		bool shorter = (projectedVector.getLength() < this->getVelocity().getLength());
		bool inRadius = oc.getLength() < unit2->getRadius() + this->getRadius() + EPSILON;

		/*/
		//do something about collision
		/*/// *
		if (shorter && inRadius)
		{
		//set move vector along [CollisionPoint -  O] at the distance R + r + e
			Vector2D normalOC = oc;
			normalOC.normalize();

			normalOC = normalOC * (unit2->getRadius() + this->getRadius() + EPSILON);
			Vector2D desiredPoint = (normalOC + oc + this->getVelocity());

			this->setVelocity(desiredPoint);

		}//*/
	}
	
}

void KinematicUnit::handleWallCollisions(float time)
{
	std::vector<Wall> walls = gpGame->getWalls();
	for (size_t i = 0; i < walls.size(); i++)
	{
		//Vector2D tmp = walls[i].getBounceVector(this->getPosition(), this->getVelocity());
		//check collisions
		if (walls[i].checkLineCollision(this->getPosition(), this->getVelocity() * time))
		{
			//bounce it like a polaroid
			//Vector
			Vector2D bounce = walls[i].getBounceVector(this->getPosition(), this->getVelocity() * time);
			setVelocity(bounce);
		}
	}

}

Vector2D KinematicUnit::handleWallAvoidance()
{
	std::vector<Wall> walls = gpGame->getWalls();

	Vector2D accelVector = Vector2D();
	for each (Wall wall in walls)
	{
		//check the distance from player to wall
		accelVector += wall.getWallForce(getPosition(), getMaxAcceleration());

	}
	return accelVector;
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
	DynamicWanderSteering* pWander = new DynamicWanderSteering(this);
	setSteering(pWander);
	//KinematicWanderSteering* pWanderSteering = new KinematicWanderSteering( this );
	//setSteering( pWanderSteering );
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
	//WanderAndSeekSteering* pWanderAndFlee = new WanderAndSeekSteering(this, pTarget, true);
	//setSteering(pWanderAndFlee);
}
void KinematicUnit::dynamicWanderSeek(KinematicUnit* pTarget)
{
	//WanderAndSeekSteering* pWanderAndSeek = new WanderAndSeekSteering(this, pTarget, false);
	//setSteering(pWanderAndSeek);
}

void KinematicUnit::flock(KinematicUnit* pTarget)
{
	FlockingSteering* pFlock = new FlockingSteering(this, pTarget);
	setSteering(pFlock);
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
		case FLOCK:
			this->flock(pTarget);
			break;
		default:
			return false;
			break;
	}
	return true;
}
