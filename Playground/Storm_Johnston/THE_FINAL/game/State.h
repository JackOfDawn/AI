#pragma once

#include "MapHolder.h"
#include "KinematicUnit.h"
#include <Sprite.h>
#include <GraphicsBuffer.h>
#include <allegro5\allegro_color.h>


//Colors
//const ALLEGRO_COLOR patrolColor = al_map_rgb(0, 128, 0);

class Grue;
class Player;

const int AGRO_DIST = 150;

class GrueState
{
public:
	//States

	virtual ~GrueState() {}
	virtual void update(Grue& grue, Player& player) = 0;
	virtual void draw(Sprite* sprite, GraphicsBuffer* buffer, Vector2D& position, float orientation, float scale) {}
};


class PatrolState : public GrueState
{
public:
	
	virtual void update(Grue& grue, Player& player);
	virtual void draw(Sprite* sprite, GraphicsBuffer* buffer, Vector2D& position, float orientation, float scale);
	
};

class FleeState : public GrueState
{
	virtual void update(Grue& grue, Player& player);
	virtual void draw(Sprite* sprite, GraphicsBuffer* buffer, Vector2D& position, float orientation, float scale);
};

class ChaseState : public GrueState
{
	virtual void update(Grue& grue, Player& player);
	virtual void draw(Sprite* sprite, GraphicsBuffer* buffer, Vector2D& position, float orientation, float scale);
};



class State
{
public: 
	static PatrolState patrol;
	static FleeState flee;
	static ChaseState chase;

};