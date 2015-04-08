#include "UnitManager.h"
#include "KinematicUnit.h"
#include "Sprite.h"
#include "Game.h"
#include "Player.h"
#include "Grue.h"
#include "Coin.h"
#include "MightyCandy.h"
#include "State.h"
#include "MapHolder.h"
#include "SpriteManager.h"
#include "GameMessageManager.h"
#include "GameApp.h"
#include "GridGraph.h"


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

bool UnitManager::createPlayerUnit(Sprite* pSprite)
{
	//Get player spawn info from the MapHolder
	SpawnInfo spawnInfo = gpMapHolder->getPlayerSpawn();
	if (spawnInfo.mapID != -1)
	{
		Player* tmp = new Player(pSprite, spawnInfo);
		tmp->LoadIn();
		mpPlayer = tmp;
		mpPlayer->dynamicSeek();
		addUnit(tmp);
		return true;
	}
	return false;
}
	

bool UnitManager::createAIUnits(Sprite* pSprite)
{
	std::vector<SpawnInfo> grueSpawns = gpMapHolder->getGrueSpawns();

	if (grueSpawns.size() > 0)
	{
		for each (SpawnInfo spawn in grueSpawns)
		{
			Grue* tmpGrue = new Grue(pSprite);
			tmpGrue->LoadIn(spawn, mpPlayer);
			tmpGrue->dynamicSeek();
			addUnit(tmpGrue);
		}
		return true;
	}

	return false;
}

bool UnitManager::createCandies(Sprite* pSprite)
{
	std::vector<SpawnInfo> candySpawns = gpMapHolder->getCandySpawns();

	if (candySpawns.size() > 0)
	{
		for each (SpawnInfo spawn in candySpawns)
		{
			MightyCandy* candy = new MightyCandy(pSprite);
			candy->loadIn(spawn);
			addUnit(candy);
		}
		return true;
	}

	return false;
}

bool UnitManager::createCoins(Sprite* pSprite, int maxCoins, float percent)
{
	for (int i = 0; i < maxCoins; ++i)
	{
		float randNum = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

		if (randNum < percent)
		{
			EndPoint point = gpMapHolder->generateValidTarget();
			GridGraph* tmp = gpMapHolder->getGridGraph(point.mapID);
			Vector2D location = tmp->getLocation(tmp->getNode(point.index));
			location += tmp->getSquareSize()/2;

			Coin* coin = new Coin(location, pSprite);
			coin->setMapID(point.mapID);
			addUnit(coin);
		}
	}

	return true;
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

	KinematicUnit* collisionUnit = checkPlayerCollisions();

	if (collisionUnit != NULL)
	{
		handlePlayerCollisions(checkPlayerCollisions());
	}
}

void UnitManager::draw(GraphicsBuffer* pBackBuffer)
{
	map<IDType, KinematicUnit*>::iterator iter;
	//update each unit
	for (iter = mUnits.begin(); iter != mUnits.end(); ++iter)
	{
		if (iter->second->getMapID() == gpMapHolder->getCurrentMap()->getID())
		iter->second->draw(pBackBuffer);
	}
}

void UnitManager::drawDebugInfo(GraphicsBuffer* pBackBuffer)
{
	map<IDType, KinematicUnit*>::iterator iter;
	//update each unit
	for (iter = mUnits.begin(); iter != mUnits.end(); ++iter)
	{
		if (iter->second->getMapID() == gpMapHolder->getCurrentMap()->getID() )
			iter->second->visualizePath(gpMapHolder->getCurrentMap()->getGrid(), pBackBuffer);
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

KinematicUnit* UnitManager::checkPlayerCollisions()
{
	auto itr = mUnits.begin();

	if (!mpPlayer->isDead())
	{
		for (; itr != mUnits.end(); ++itr)
		{
			if(itr->second->getMapID() == mpPlayer->getMapID() && itr->second->getType() != mpPlayer->getType() && !itr->second->isDead())
			{
				int radius = mpPlayer->getSizeScaled();

				if (radius > Vector2D::distance(mpPlayer->getPosition(), itr->second->getPosition()))
				{
					return itr->second;
				}
			}
		}
	}

	return NULL;
}

bool UnitManager::handlePlayerCollisions(KinematicUnit* collider)
{
	Coin* coin = (Coin*)collider;
	Grue* grue = (Grue*)collider;

	switch (collider->getType())
	{
	case COIN:
		mpPlayer->eat();
		((GameApp*)gpGame)->addScore(coin->getValue());
		deleteUnit(coin);
		break;
	case GRUE:
		if (mpPlayer->getInvincibility() <= 0.0f)
		{
			((GameApp*)gpGame)->addScore(-25);
			((GameApp*)gpGame)->addDeath(1);
			killUnitTime(mpPlayer, 5);
		}
		else
		{
			((GameApp*)gpGame)->addScore(grue->getValue());
			((GameApp*)gpGame)->addGrue(1);
			killUnitTime(collider, 10);
		}
		break;
	case CANDY:
		mpPlayer->resetInvincibility();
		((GameApp*)gpGame)->addCandy(1);
		killUnitTime(collider, 60);
		break;
	}

	return true;
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

bool UnitManager::killUnitTime(KinematicUnit* toKill, float timeToRespawn)
{
	return toKill->dieTill(timeToRespawn);
}