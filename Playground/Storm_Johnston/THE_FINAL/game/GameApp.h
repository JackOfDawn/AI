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
class Grue;
class GameMessageManager;
class Grid;
class GridVisualizer;
class GridGraph;
class GridPathfinder;
class DebugDisplay;
class InputManager;
class MapHolder;
class UnitManager;
class HUD;

class GameApp : public Game
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
	inline HUD* getHUD()  { return mpHUD; }
	inline UnitManager* getUnitManager() {return mpUnitManager;};
	GridVisualizer* getGridVisualizer();
	GridPathfinder* getPathfinder();
	Grid* getGrid();
	GridGraph* getGridGraph();
	inline void startGame() { onSplash = false; };
	inline void toggleDebugMode() { debugMode = !debugMode; };

	void updateUnitPath(std::vector<Vector2D> path, bool reversed);

	void addScore(int num){ mScore += num; }
	int getScore(){ return mScore; }
	void addGrue(int num){ mGruesEaten += num; }
	int getGrue(){ return mGruesEaten; }
	void addDeath(int num){ mDeaths += num; }
	int getDeaths(){ return mDeaths; }
	void addCandy(int num){ mCandies += num; }
	int getCandies(){ return mCandies; }

private:
	GameMessageManager* mpMessageManager;

	DebugDisplay* mpDebugDisplay;
	

	InputManager* mpInputHandler;


	UnitManager* mpUnitManager;
	HUD* mpHUD;
	ALLEGRO_FONT* mpFont;

	bool debugMode;
	bool onSplash;


	int mScore;
	int mGruesEaten;
	int mDeaths;
	int mCandies;
};

