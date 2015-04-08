#pragma once

#include "Game.h"
#include "../game/KinematicUnit.h"
#include "Grid.h"
#include <string>
#include "PortalInformation.h"
#include <vector>
#include <unordered_map>

class Grid;
class GridVisualizer;

class Map : public Trackable
{
public:
	Map();
	~Map();

	void initNewMap(const int width, const int height, const int cellSize, const int id);


	inline Grid* getGrid() { return mpGrid; };
	inline GridVisualizer* getGridVisualizer() { return mpGridVisualizer; };
	inline int getID() { return mID; }
	inline std::string getFileName() { return mMapFile; }
	void saveGrid(std::ofstream& theStream);
	void loadGrid(std::ifstream& theStream);

	void setLinks(PortalInfo* pPortals);
	void addGrue(int x, int y);
	void removeGrue(int x, int y);
	void addCandy(int x, int y);
	void removeCandy(int x, int y);
	void setPlayer(int location) { mPlayerSpawn = location; };
	inline int getPlayer() const { return mPlayerSpawn; };
	float getScale(){ return (mpGrid->getSquareSize() / 12.0f); } //Janky

	inline std::vector<int> getGrues() { return mGrueSpawns; };
	inline std::vector<int> getCandy() { return mCandySpawns; };

	
	std::vector<PortalLinks> getLinks() { return mLinks; };

private:
	Grid* mpGrid;
	GridVisualizer* mpGridVisualizer;
	std::string mMapFile;
	int mID;
	

	std::vector<PortalLinks> mLinks;
	std::vector<int> mGrueSpawns;
	std::vector<int> mCandySpawns;
	int mPlayerSpawn;
};

