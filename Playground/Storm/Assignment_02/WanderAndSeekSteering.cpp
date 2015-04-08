#include "WanderAndSeekSteering.h"
#include "KinematicUnit.h"
#include "Game.h"


WanderAndSeekSteering::WanderAndSeekSteering(KinematicUnit* pMover, KinematicUnit* pTarget, bool shouldFlee)
:mpMover(pMover)
,mpTarget(pTarget)
,mShouldFlee(shouldFlee)
{
	mApplyDirectly = false;
	mShouldWander = true;
	mWanderOrientation = genRandomBinomial() * MAX_DYNAMIC_WANDER_ROTATION;
}

Steering* WanderAndSeekSteering::getSteering()
{

	std::vector<Rect2D> walls = checkWalls();

	if (walls.size() > 0)
	{
		
		//get closest wall
		int vectorLoc = 0;
		Rect2D currentWall = walls[0];
		Vector2D pos = mpMover->getPosition();

			
		if (walls.size() > 1)
		{
			for (size_t i = 0; i < walls.size() - 1; i++)
			{
				if (walls[i].getDistanceToEdge(pos) <= walls[i + 1].getDistanceToEdge(pos))
				{
					currentWall = walls[i];
					vectorLoc = i;
				}
			}
		}
			
		Vector2D edgeVector = currentWall.getEdgeLine(pos);

		
		//get the normal
		Vector2D normal = Vector2D(-edgeVector.getY(), edgeVector.getX());
		normal.normalize();

		//dot product with normal and velocity
		float distanceAlongNormal;
		{
			distanceAlongNormal = edgeVector.getX()*mpMover->getVelocity().getX() + edgeVector.getY()*mpMover->getVelocity().getY();
		}

		mLinear = (normal * (2.0 * distanceAlongNormal)) - mpMover->getPosition();

		//find the angle?
		//mAngular = acosf(distanceAlongNormal / (mpMover->getVelocity().getLength() * edgeVector.getLength()));
		//mAngular = 

		//erase wall after bounce
		walls.erase(walls.begin() + vectorLoc);
			
	

		mApplyDirectly = true; //this is uh shitty and the only way it somewhat works
	}
	else
	{
		mApplyDirectly = false;
		checkIfShouldWander();

		//wander
		if (mShouldWander)
		{
			wander();
		}
		else
		{
			seek();
		}

		
		mAngular = 0;
	}


	mLinear.normalize();
	mLinear *= mpMover->getMaxVelocity();

	return this;
}

std::vector<Rect2D> WanderAndSeekSteering::checkWalls()
{
	std::vector<Rect2D> closeWalls = std::vector<Rect2D>();
	std::vector<Rect2D> walls = gpGame->getWalls();

	for (int i = 0; i < walls.size(); i++)
	{
		float distance = walls[i].getDistanceToEdge(mpMover->getPosition());
		bool closeEnough = (distance <= BOUNCE_RADIUS);
		if (closeEnough)
		{
			closeWalls.push_back(walls[i]);
		}
	}

	return closeWalls;

}

void WanderAndSeekSteering::wander()
{
	//1.calculate the target to delegate to face

	//update wander orientation 
	mWanderOrientation += genRandomBinomial() * MAX_DYNAMIC_WANDER_ROTATION;

	//calculate the combined target orientation
	float	targetOrientation = mWanderOrientation + mpMover->getOrientation();

	//Calculate center of wander circle

	Vector2D target = mpMover->getPosition() + (mpMover->getOrientationAsVector() * WANDER_FORWARD_OFFSET);

	//calculate target location
	Vector2D targetOffset = (asVector(targetOrientation) * WANDER_RADIUS);
	target = target + targetOffset;

	//set the acceleration to be at full
	mLinear = target - mpMover->getPosition();

	

}


Vector2D WanderAndSeekSteering::asVector(float orientation)
{
	return Vector2D(sinf(orientation), cosf(orientation));
}

void WanderAndSeekSteering::seek()
{
	if (!mShouldFlee)
	{
		mLinear = mpTarget->getPosition() - mpMover->getPosition();
	}
	else
	{
		mLinear = mpMover->getPosition() - mpTarget->getPosition();
	}	

}

void WanderAndSeekSteering::checkIfShouldWander()
{
	float	distanceX = (mpMover->getPosition().getX() - mpTarget->getPosition().getX()),
			distanceY = (mpMover->getPosition().getY() - mpTarget->getPosition().getY());

	bool isCloseEnough = (distanceX*distanceX) + (distanceY*distanceY) < MINIMUM_DISTANCE * MINIMUM_DISTANCE;

	if (isCloseEnough) mShouldWander = false;
	else mShouldWander = true;
}