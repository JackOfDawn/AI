#include "UnitManager.h"
#include "GameMessage.h"

UnitManager::UnitManager()
{
}

UnitManager::~UnitManager()
{
	for (int i = 0; i < (signed)mUnits.size(); i++)
	{
		delete mUnits[i];
		mUnits[i] = NULL;
	}
}

int UnitManager::getSize()
{
	return mUnits.size();
}

KinematicUnit* UnitManager::getUnit(int unit)
{
	if ((signed)mUnits.size() > unit)
	{
	return mUnits[unit];
	}
	else
		return NULL;
}

void UnitManager::addUnit(KinematicUnit* unit)
{
	mUnits.push_back(unit);
}

void UnitManager::removeUnit(int unit)
{
	if (unit < 0)
	{
		return;
	}

	mUnits.erase(mUnits.begin() + unit);
}

void UnitManager::init()
{
	mUnits = vector<KinematicUnit*>();
}

void UnitManager::update(float time)
{
	// update all the units
	for (int i = 0; i < (signed)mUnits.size(); i++)
	{
		mUnits[i]->update(time);
	}
}

vector<KinematicUnit*> UnitManager::getUnitsInRadius(Vector2D position, int radius)
{
	vector<KinematicUnit*> units = vector<KinematicUnit*>();

	for (int i = 0; i < (signed)mUnits.size(); i++)
	{
		if (pow(mUnits[i]->getPosition().getX() - position.getX(), 2) + pow(mUnits[i]->getPosition().getY() - position.getY(), 2) < pow((double)radius, 2))
		{
			units.push_back(mUnits[i]);
		}
	}

	return units;
}

void UnitManager::draw(GraphicsBuffer* buffer)
{
	for (int i = 0; i < (signed)mUnits.size(); i++)
	{
		mUnits[i]->draw(buffer);
	}
}