#include "Game.h"
#include "Kinematic.h"
#include "Sprite.h"
#include "GraphicsSystem.h"
#include "Defines.h"
#include "Asteroid.h"
#include "UnitManager.h"
#include "Wall.h"

Asteroid::Asteroid(Sprite* pSprite, const Vector2D& position, float orientation, const Vector2D &velocity, float rotationVel, float maxVelocity, float maxAcceleration, float radius)
:Kinematic(position, orientation, velocity, rotationVel)
{
	mMaxAcceleration = maxAcceleration;
	mMaxVelocity = maxVelocity;
	mRadius = radius;
	mpSprite = pSprite;
	//mRotationVel = rotationVel;
	//mOrientation = orientation;
}

void Asteroid::update(float time)
{
	//right now do nothing

	//when I decide to have these move
	//BOUNCE THEM ROIDS
	handleCylinderCollisions();

	Kinematic::capVelocity(40);
	Kinematic::update(time);

	//make them warp for asteroidfield effect
	GRAPHICS_SYSTEM->wrapCoordinates(mPosition);
	
}

void Asteroid::draw(GraphicsBuffer* pBuffer)
{
	mpSprite->draw(*pBuffer, mPosition.getX(), mPosition.getY(), mOrientation);
}

void Asteroid::handleCylinderCollisions()
{
	std::vector<Asteroid*> asteroids = UNIT_MANAGER->getAsteroids();
	for (size_t i = 0; i < asteroids.size(); i++)
	{
		if (asteroids.at(i) != this)
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
}