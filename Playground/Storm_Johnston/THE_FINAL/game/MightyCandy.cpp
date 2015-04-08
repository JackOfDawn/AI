#include "MightyCandy.h"
#include "MapHolder.h"
#include "GridGraph.h"
#include <Sprite.h>

MightyCandy::MightyCandy(Sprite* pSprite)
: KinematicUnit(pSprite, Vector2D(), 0.01f, Vector2D(), 0.0f)
{
	mType = CANDY;
}

MightyCandy::~MightyCandy()
{
}

void MightyCandy::loadIn(SpawnInfo spawn)
{
	mSpawnInfo = spawn;
	mpCurrentMap = gpMapHolder->getMap(mSpawnInfo.mapID);
	mCurrentMapID = spawn.mapID;
	GridGraph* tmp = gpMapHolder->getGridGraph(mSpawnInfo.mapID); //Do we need to store the grid graph?
	mPosition = tmp->getLocation(tmp->getNode(mSpawnInfo.location)) + tmp->getSquareSize() / 2;
	mpCurrentGridGraph = tmp;
}

void MightyCandy::livingDraw(GraphicsBuffer* pBuffer)
{
	mpSprite->draw(*pBuffer, mPosition.getX(), mPosition.getY(), mOrientation, getScale());
}