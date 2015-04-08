#pragma once

#include <vector>
#include "Sprite.h"

struct Wall
{
public:
	int	x,
		y,
		width,
		height;
	Wall(int dX, int dY, int dWidth, int dHeight) : x(dX), y(dY), width(dWidth), height(dHeight){}
	Wall(){ x = 0; y = 0; width = 0; height = 0; }
};

class GraphicsBuffer;
class KinematicUnit;

class CollisionEngine
{

public:
	CollisionEngine(Sprite* pSprite);
	~CollisionEngine();

	void update();
	void drawWalls(GraphicsBuffer* pBuffer);

private:
	Sprite* mpWallSprite;
};