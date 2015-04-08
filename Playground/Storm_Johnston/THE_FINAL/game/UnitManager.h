#pragma once

#include <map>
#include <string>
#include "Vector2D.h"
#include "Trackable.h"
#include "Defines.h"
#include "KinematicUnit.h"
#include "Player.h"


using namespace std;

class KinematicUnit;
class Sprite;

//const IDType STARTING_SEQUENTIAL_ID = 0;
//const IDType ENDING_SEQUENTIAL_ID = 999;

class UnitManager : public Trackable
{
public:
	UnitManager();
	~UnitManager();

	void init();
	void clear();

	IDType addUnit(KinematicUnit*);
	IDType deleteUnit(KinematicUnit*);
	IDType deleteUnitID(IDType);
	
	bool createPlayerUnit(Sprite* pSprite);
	bool createAIUnits(Sprite* pSprite);
	bool createCandies(Sprite* pSprite);
	bool createCoins(Sprite* pSprite, int maxCoins, float percent);

	//void setBehavior(IDType id, SteeringType behavior);

	KinematicUnit* getUnit(IDType id);
	inline Player* getPlayer() { return mpPlayer; }

	void update(float);
	void draw(GraphicsBuffer* pBackBuffer);
	void drawDebugInfo(GraphicsBuffer* pBackBuffer);

	void deleteAIUnit();
	KinematicUnit* checkPlayerCollisions();
	bool handlePlayerCollisions(KinematicUnit* collider);
	bool killUnitTime(KinematicUnit* toKill, float timeToRespawn);

private:
	IDType getNextID();

	map<IDType, KinematicUnit*> mUnits;
	Player* mpPlayer;

	static int msNextID;

};