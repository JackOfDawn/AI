#pragma once

#include "Trackable.h"
#include "PerformanceTracker.h"
#include "Vector2D.h"
#include "Defines.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <string>
#include <vector>
#include "Rect2D.h"


class GraphicsSystem;
class GraphicsBuffer;
class GraphicsBufferManager;
class SpriteManager;
class KinematicUnit; 
class GameMessageManager;
class UnitManager;
class Timer;
class InputManager;
class Vector2D;

const IDType BACKGROUND_SPRITE_ID = 0;
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;

const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame

class Game :public Trackable
{
public:
	Game();
	~Game();

	bool init();//returns true if no errors, false otherwise
	void cleanup();

	//game loop
	void beginLoop();
	void processLoop();
	bool endLoop();

	inline GraphicsSystem* getGraphicsSystem() const { return mpGraphicsSystem; };
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline InputManager* getInputManager() { return mpInputManager; };
	inline UnitManager* getUnitManager() { return mpUnitManager; };
	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline ALLEGRO_FONT* getFont() const { return mpFont; };
	inline std::vector<Rect2D> getWalls() const { return mWalls; };
	
	inline void setMouseLocation(Vector2D &pos) { mMousePos = pos; };
	inline void endGame() { mShouldExit = true; }

	void drawMouseLocation();


private:
	GraphicsSystem* mpGraphicsSystem;
	GraphicsBufferManager* mpGraphicsBufferManager;
	SpriteManager* mpSpriteManager;
	GameMessageManager* mpMessageManager;
	UnitManager* mpUnitManager;
	InputManager* mpInputManager;

	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	bool mShouldExit;
	Vector2D mMousePos;
	std::vector<Rect2D> mWalls;

	//should be somewhere else
	ALLEGRO_FONT* mpFont;
	ALLEGRO_SAMPLE* mpSample;
	IDType mBackgroundBufferID;
	IDType mPlayerIconBufferID;
	IDType mEnemyIconBufferID;

	//KinematicUnit* mpUnit;
	//KinematicUnit* mpAIUnit;
	//KinematicUnit* mpAIUnit2;
};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge


Vector2D genLocation(Vector2D &centralPos, float distance); //generates a location 

extern Game* gpGame;
extern PerformanceTracker* gpPerformanceTracker;

