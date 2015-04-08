#include "Grue.h"
#include "MapHolder.h"
#include "AStarPathFinder.h"
#include "GridGraph.h"
#include "Grid.h"
#include "Node.h"
#include <Sprite.h>
#include "State.h"
#include "Player.h"


PatrolState State::patrol = PatrolState();
ChaseState State::chase = ChaseState();
FleeState State::flee = FleeState();

Grue::Grue(Sprite* pSprite)
: KinematicUnit(pSprite, Vector2D(), 0.01f, Vector2D(), 0.0f, 80.0f, 1750.0f)
{
	mNeedsNewPath = false;
	mCurrentState = &State::patrol;
	mType = GRUE;
	mValue = 10;
}

Grue::~Grue()
{
	//delete mpPathfinder;
}

void Grue::LoadIn(SpawnInfo spawn, KinematicUnit* pPlayer)
{
	mSpawnInfo = spawn;
	mpPlayer = pPlayer;
	mpCurrentMap = gpMapHolder->getMap(mSpawnInfo.mapID);
	mCurrentMapID = spawn.mapID;
	GridGraph* tmp = gpMapHolder->getGridGraph(mSpawnInfo.mapID); //Do we need to store the grid graph?
	mpPathfinder = new AStarPathfinder(tmp);
	mPosition = tmp->getLocation(tmp->getNode(mSpawnInfo.location)) + tmp->getSquareSize()/2;
	mpCurrentGridGraph = tmp;
}

void Grue::livingUpdate(float time)
{
	//GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	mCurrentState->update(*this, *(dynamic_cast<Player*>(mpPlayer)));

	if (mPathingInfo.isValid())
	{
		if (mNeedsNewPath)
		{
			findNewPath();
		}

		KinematicUnit::livingUpdate(time);
	}
}
	
void Grue::livingDraw(GraphicsBuffer* pBuffer)
{
	mCurrentState->draw(mpSprite, pBuffer, mPosition, mOrientation, getScale());
}

