#include "UnitManager.h"
#include "KinematicUnit.h"
#include "Sprite.h"
#include "Game.h"
#include "SpriteManager.h"
#include "EndGameMessage.h"
#include "GameMessageManager.h"


int UnitManager::msNextID = 0;

UnitManager::UnitManager()
{

}

UnitManager::~UnitManager()
{
	clear();
}

void UnitManager::init()
{
	mpPlayer = NULL;
	mAsteroids = vector<Asteroid*>();
}

void UnitManager::clear()
{
	map<IDType, KinematicUnit*>::iterator iter;
	for (iter = mUnits.begin(); iter != mUnits.end(); ++iter)
	{
		KinematicUnit* pUnit = iter->second;
		delete pUnit;
	}
	mUnits.clear();

	for (int i = mAsteroids.size() - 1; i >= 0; i--)
	{
		Asteroid* pAsteroid = mAsteroids.at(i);
		delete pAsteroid;
	}
	mAsteroids.clear();
}

IDType UnitManager::addUnit(KinematicUnit* unit)
{
	map<IDType, KinematicUnit*>::iterator iter;
	//find if it already is there
	for (iter = mUnits.begin(); iter != mUnits.end(); ++iter)
	{
		if (iter->second == unit)
		{
			return INVALID_ID;
		}
	}

	IDType theID = getNextID();
	//otherwise add the new unit
	mUnits[theID] = unit;
	unit->setID(theID);
	return theID;

}

IDType UnitManager::deleteUnit(KinematicUnit* unit)
{
	IDType theID = INVALID_ID;
	map<IDType, KinematicUnit*>::iterator iter;
	//find if it already is there
	for (iter = mUnits.begin(); iter != mUnits.end(); ++iter)
	{
		if (iter->second == unit)
		{
			theID = iter->first;
			KinematicUnit* pUnit = iter->second;
			delete pUnit;
			mUnits.erase(iter);
			break;
		}
	}

	return theID;
}

IDType UnitManager::deleteUnitID(IDType id)
{
	KinematicUnit* pUnit = mUnits[id];
	delete pUnit;
	mUnits.erase(id);
	pUnit = NULL;
	return id;
	
}

bool UnitManager::createPlayerUnit(Sprite* pSprite, const Vector2D &position, float orientation, const Vector2D &velocity, float rotationVel,
	float maxVelocity, float maxAcceleration, SteeringType SteeringBehavior)
{
	//if (mpPlayer != NULL) return false; //checks if player has already been made;
	
	mpPlayer = new KinematicUnit(pSprite, position, orientation, velocity, rotationVel, maxVelocity, maxAcceleration);
	mpPlayer->setDynamicSteering(SteeringBehavior, mpPlayer);
	addUnit(mpPlayer);
	return true;
}

bool UnitManager::createAIUnit(Sprite* pSprite, const Vector2D &position, float orientation, const Vector2D &velocity, float rotationVel,
	float maxVelocity, float maxAcceleration, SteeringType SteeringBehavior)
{
	KinematicUnit* tmpUnit = new KinematicUnit(pSprite, position, orientation, velocity, rotationVel, maxVelocity, maxAcceleration);
	tmpUnit->setDynamicSteering(SteeringBehavior, mpPlayer);
	addUnit(tmpUnit);
	return false;
}

bool UnitManager::createAsteroid(IDType id)
{
	bool success = true;

	Sprite* pAsteroidSprite = NULL;
	Vector2D velocity = Vector2D(rand() % 50 + 50, rand() % 50 + 50);
	velocity.setXY(velocity.getX() * genRandomBinomial(), velocity.getY() * genRandomBinomial());
	int radius;
	switch (id)
	{
	case SMALL_ASTEROID_ID:
		pAsteroidSprite = SPRITE_MANAGER->getSprite(SMALL_ASTEROID_ID);
		radius = 8;
		break;
	case LARGE_ASTEROID_ID:
		pAsteroidSprite = SPRITE_MANAGER->getSprite(LARGE_ASTEROID_ID);
		radius = 32;
		break;
	case MEDIUM_ASTEROID_ID:
		pAsteroidSprite = SPRITE_MANAGER->getSprite(MEDIUM_ASTEROID_ID);
		radius = 16;
		break;
	default:
		success = false;
		break;
	}

	if (success)
	{
		Vector2D newPos = Vector2D((rand() % 400) + 100, (rand() % 300 + 100));
		Asteroid* newAsteriod = new Asteroid(pAsteroidSprite, newPos, 0.f, velocity, .5f, 20.0f, 2.0f, radius);
		mAsteroids.push_back(newAsteriod);
	}

	return success;
}

KinematicUnit* UnitManager::getUnit(IDType id)
{
	if (mUnits[id] == NULL)
	{
		mUnits.erase(id);
		return NULL;
	}
	else
	{
		return mUnits[id];
	}
}

void UnitManager::update(float time)
{

	map<IDType, KinematicUnit*>::iterator iter;
	//update each unit
	for (iter = mUnits.begin(); iter != mUnits.end(); ++iter)
	{
		//if (iter->second != NULL)
			iter->second->update(time);
	}
	for (size_t i = 0; i < mAsteroids.size(); i++)
	{
		mAsteroids.at(i)->update(time);
	}

}

void UnitManager::draw(GraphicsBuffer* pBackBuffer)
{
	map<IDType, KinematicUnit*>::iterator iter;
	//update each unit
	for (iter = mUnits.begin(); iter != mUnits.end(); ++iter)
	{
		iter->second->draw(pBackBuffer);
	}

	for (size_t i = 0; i < mAsteroids.size(); i++)
	{
		mAsteroids.at(i)->draw(pBackBuffer);
	}
}

void UnitManager::addDynamicArrive(float distance)
{
	Vector2D location = mpPlayer->getPosition();
	
	//generate location based on player location and distance
	location = genLocation(location, distance);
	//call create AI Unit with Dynamic Arrive
	createAIUnit(SPRITE_MANAGER->getSprite(AI_ICON_SPRITE_ID), location, 1, gZeroVector2D, 0.0f, 180.0f, 100.0f, DYNAMIC_ARRIVE);
}

void UnitManager::addDynamicSeek(float distance)
{
	Vector2D location = mpPlayer->getPosition();

	//generate location based on player location and distance
	location = genLocation(location, distance);
	//call create AI Unit with Dynamic Seek
	createAIUnit(SPRITE_MANAGER->getSprite(AI_ICON_SPRITE_ID), location, 1, gZeroVector2D, 0.0f, 180.0f, 100.0f, DYNAMIC_SEEK);
}

void UnitManager::addWanderSeek(float distance)
{
	Vector2D location = mpPlayer->getPosition();

	//generate location based on player location and distance
	location = genLocation(location, distance);

	createAIUnit(SPRITE_MANAGER->getSprite(AI_ICON_SPRITE_ID), location, 1, gZeroVector2D, 0.0f, 180.0f, 100.0f, DYNAMIC_WANDER_SEEK);
}

void UnitManager::addWanderFlee(float distance)
{
	Vector2D location = mpPlayer->getPosition();

	//generate location based on player location and distance
	location = genLocation(location, distance);

	createAIUnit(SPRITE_MANAGER->getSprite(AI_ICON_SPRITE_ID), location, 1, gZeroVector2D, 0.0f, 180.0f, 100.0f, DYNAMIC_WANDER_FLEE);
}

void UnitManager::addFlock()
{
	Vector2D location = Vector2D(300, 400);
	float distance = 100;
	const int NUM_BOIDS = 5;
	Vector2D spawnLocation;

	for (size_t i = 0; i < NUM_BOIDS; i++)
	{
		spawnLocation = genLocation(location, distance);
		createAIUnit(SPRITE_MANAGER->getSprite(AI_ICON_SPRITE_ID), spawnLocation, 1, gZeroVector2D, 0.0f, 180.0f, 100.0f, FLOCK);
	}
}

void UnitManager::deleteAIUnit()
{
	//checks if it is empty or not
	//if (mUnits.count == 0) return;
	if (mUnits.size() == 1 && mpPlayer != NULL) return;

	//get an index
	int index = rand() % msNextID;
	KinematicUnit* tmpUnit = getUnit(index);

	//if id does not exist or if it is player find a new one
	while (tmpUnit == NULL || tmpUnit == mpPlayer)
	{
 		index = rand() % msNextID;
		tmpUnit = getUnit(index);
	}

	//delete it;
	tmpUnit = NULL;
	deleteUnitID(index);
}

IDType UnitManager::getNextID()
{
	IDType theID = INVALID_ID;

	//find an unused ID
	for (int i = msNextID; i < HIGHEST_ID; i++)
	{
		map<IDType, KinematicUnit*>::iterator iter = mUnits.find(i);
		if (iter == mUnits.end())//not used
		{
			theID = i;
			msNextID = i + 1;
			break;
		}
	}
	return theID;
}

std::vector<KinematicUnit*> UnitManager::getNeighbors(IDType unitID)
{
	std::vector<KinematicUnit*> neighbors = std::vector<KinematicUnit*>();
	Vector2D pos = getUnit(unitID)->getPosition();
	Vector2D distanceVector;


	map<IDType, KinematicUnit*>::iterator iter;
	for (iter = mUnits.begin(); iter != mUnits.end(); ++iter)
	{
		distanceVector = iter->second->getPosition() - pos;

		if (abs(distanceVector.getLength()) < NEIGHBOR_RADIUS && iter->second->getID() != unitID)
		{
			neighbors.push_back(iter->second);
		}
	}

	return neighbors;
}

void UnitManager::deleteAsteroid()
{
	if (mAsteroids.size() >= 1)
	{
		int selected = rand() % mAsteroids.size();
		delete mAsteroids.at(selected);
		mAsteroids.erase(mAsteroids.begin() + selected);
	}

	
}
