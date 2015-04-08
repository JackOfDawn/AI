#pragma once

#include <map>
#include <string>
#include "Vector2D.h"
#include "Trackable.h"
#include "Defines.h"
#include "KinematicUnit.h"
#include "CollisionEngine.h"
#include "Asteroid.h"
#include <vector>

using namespace std;

class KinematicUnit;
class Sprite;

//const IDType STARTING_SEQUENTIAL_ID = 0;
//const IDType ENDING_SEQUENTIAL_ID = 999;
static const int NEIGHBOR_RADIUS = 200;

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
	
	bool createPlayerUnit(Sprite* pSprite,  const Vector2D &position, float orientation, const Vector2D &velocity, float rotationVel, float maxVelocity, float maxAcceleration, SteeringType SteeringBehavior);
	bool createAIUnit(Sprite* pSprite,  const Vector2D &position, float orientation, const Vector2D &velocity, float rotationVel, float maxVelocity, float maxAcceleration, SteeringType SteeringBehavior);
	bool createAsteroid(IDType asteriodType);
	//void setBehavior(IDType id, SteeringType behavior);

	KinematicUnit* getUnit(IDType id);
	inline KinematicUnit* getPlayer() { return mpPlayer; }
	std::vector<KinematicUnit*> getNeighbors(IDType unitID);
	inline std::vector<Asteroid*> getAsteroids() { return mAsteroids;}
	void deleteAsteroid();

	void update(float);
	void draw(GraphicsBuffer* pBackBuffer);

	//message events
	void addDynamicArrive(float distance);
	void addDynamicSeek(float distance);
	void addWanderSeek(float distance);
	void addWanderFlee(float distance);
	void addFlock();

	void deleteAIUnit();

private:
	IDType getNextID();

	map<IDType, KinematicUnit*> mUnits;
	std::vector<Asteroid*> mAsteroids;
	KinematicUnit* mpPlayer;

	static int msNextID;

};