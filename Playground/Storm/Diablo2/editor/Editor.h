#pragma once

#include "Game.h"
#include <fstream>
#include <vector>
#include "PortalInformation.h"

/*Editor - class to hold all application related info.

Dean Lawson
Champlain College
2011
*/

//forward declarations
class GraphicsBuffer;
class Sprite;
class Grid;
class GridVisualizer;
class GraphicsBuffer;
class Map;
class GameMessageManager;
class InputManager;
class HUD;

const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame


class Editor: public Game
{
public:
	Editor();
	virtual ~Editor();

	inline Map* getCurrentMap(){ return mpMap; }

	virtual bool init();
	virtual void cleanup();
	virtual void cleanupMaps();

	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline HUD* getHUD() { return mpHUD; };
	


	//game loop
	virtual void beginLoop();
	virtual void processLoop();
	virtual bool endLoop();


	void createNewMap();
	void removeCurrentMap();
	void changeMap(bool forward);
	
	void setBlockType(int blockType);
	void removeBlock(int x, int y);
	void addBlock(int x, int y);

	void save();
	void load();



private: 
	void removePortal(int x, int y);
	bool checkandAddPortal(int value, int index);
	void fetchCorrectMap();
	void addLinks(std::vector<PortalLinks> link);
	void resetPortals();
	void saveGrid(std::ofstream& theStream); //Make these Private
	void loadGrid(std::ifstream& theStream); //Make these Private

	int mSelectedBlock;
	Map* mpMap;
	std::vector<Map*> mMapList;
	size_t mCounterMap;
	size_t NEXT_ID;
	GameMessageManager* mpMessageManager;
	InputManager* mpInputManager;
	HUD* mpHUD;
	PortalInfo mPortals[MAX_PORTALS];
};