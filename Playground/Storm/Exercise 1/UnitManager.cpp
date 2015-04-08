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
	if (mUnits.size() == 1 && mpPlayer != NULL)
	{
		GameMessage* pMessage = new EndGameMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	map<IDType, KinematicUnit*>::iterator iter;
	//update each unit
	for (iter = mUnits.begin(); iter != mUnits.end(); ++iter)
	{
		//if (iter->second != NULL)
			iter->second->update(time);
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