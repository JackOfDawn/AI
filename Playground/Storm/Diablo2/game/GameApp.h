#pragma once

/*Game - class to hold all game related info.

Dean Lawson
Champlain College
2011
*/

#include "Game.h"
#include <vector>
#include <Vector2D.h>

//forward declarations
class GraphicsBuffer;
class Sprite;
class KinematicUnit;
class GameMessageManager;
class Grid;
class GridVisualizer;
class GridGraph;
class GridPathfinder;
class DebugDisplay;
class InputManager;
class MapHolder;
class HUD;
const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame


class GameApp: public Game
{
public:
	GameApp();
	~GameApp();

	virtual bool init();
	virtual void cleanup();

	//game loop
	virtual void beginLoop();
	virtual void processLoop();
	virtual bool endLoop();

	//accessors
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline KinematicUnit* getKinematicUnit() { return mpUnit; };
	inline HUD* getHUD()  { return mpHUD; }
	GridVisualizer* getGridVisualizer();
	GridPathfinder* getPathfinder();
	Grid* getGrid();
	GridGraph* getGridGraph();
	inline MapHolder* getMapHolder() { return mpMapHolder; };


	void updateUnitPath(std::vector<Vector2D> path, bool reversed);

private:
	GameMessageManager* mpMessageManager;

	DebugDisplay* mpDebugDisplay;
	MapHolder* mpMapHolder;

	InputManager* mpInputHandler;

	KinematicUnit* mpUnit;
	HUD* mpHUD;

};

