#include "Map.h"
#include <Grid.h>
#include <GridVisualizer.h>
#include "Vector2D.h"
#include <sstream>
#include "Defines.h"

using namespace std;



Map::Map()
{
	mpGridVisualizer = NULL;
	mMapFile = "";

	mpGrid = new Grid(0, 0, 1);
	mpGridVisualizer = new GridVisualizer(mpGrid);
	mID = INVALID_ID;
	mLinks = std::vector<PortalLinks>();
	mGrueSpawns = vector<int>();
	mCandySpawns = vector<int>();
	mPlayerSpawn = -1;
}

Map::~Map()
{
	delete mpGrid;
	mpGrid = NULL;

	delete mpGridVisualizer;
	mpGridVisualizer = NULL;
}

void Map::initNewMap(const int width, const int height, const int cellSize, const int id)
{
	delete mpGrid;
	mpGrid = new Grid(width, height, cellSize);

	delete mpGridVisualizer;
	mpGridVisualizer = new GridVisualizer(mpGrid);
	mID = id;
	
	std::stringstream fileName = stringstream();
	fileName << FILE_PREFFIX << mID << FILE_SUFFIX;
	mMapFile = fileName.str();

}


void Map::saveGrid(std::ofstream& theStream)
{
	//add the map to the maindata file
	theStream << mMapFile << endl;


	//save the grid to the file
	std::ofstream mapData;
	mapData.open(mMapFile);
	if (mapData.good())
	{
		mapData.clear();//wipe the original map
		mapData.write((char*)&mID, sizeof(int)); // adds the map ID to the file
		mpGrid->save(mapData); //save the width height and cell size of the file

		//save portal informations
		for (size_t i = 0; i < mLinks.size(); i++)
		{
			mapData.write((char*)&mLinks[i].portalID, sizeof(int));
			mapData.write((char*)&mLinks[i].map2ID, sizeof(int));
			mapData.write((char*)&mLinks[i].portalIndex, sizeof(int));
			
		}
		//save grues
		for (size_t i = 0; i < mGrueSpawns.size(); i++)
		{
			mapData.write((char*)&mGrueSpawns[i], sizeof(int));
		}
		//save candy
		for (size_t i = 0; i < mCandySpawns.size(); i++)
		{
			mapData.write((char*)&mCandySpawns[i], sizeof(int));
		}
		//save player
		if (mPlayerSpawn != -1)
		{
			mapData.write((char*)&mPlayerSpawn, sizeof(int));
		}
	}
	mapData.close();

}

void Map::loadGrid(std::ifstream& theStream)
{
	mLinks.clear();
	mGrueSpawns.clear();
	mCandySpawns.clear();
	mPlayerSpawn = INVALID_ID;
	//load map from the file
	std::string path;
	getline(theStream, path);

	std::ifstream mapData;
	mapData.open(path);

	if (mapData.good())
	{
		//load the grid to the new map
		//get the id
		mapData.read((char*)&mID, sizeof(int));
		std::stringstream fileName = stringstream();
		
		//get thefilename
		fileName << FILE_PREFFIX << mID << FILE_SUFFIX;
		mMapFile = fileName.str();
		//load the wifth height cellsize and tilemap
		mpGrid->load(mapData);

		//load the portals
		for (int i = 0; i < mpGrid->getNumPortals(); i++)
		{
			PortalLinks link;
			mapData.read((char*)&link.portalID, sizeof(int));
			mapData.read((char*)&link.map2ID, sizeof(int));
			mapData.read((char*)&link.portalIndex, sizeof(int));
			mLinks.push_back(link);
		}

		//load the grues
		for (int i = 0; i < mpGrid->getNumGrues(); i++)
		{
			int value;
			mapData.read((char*)&value, sizeof(int));
			mGrueSpawns.push_back(value);
		}
		//load the candy
		for (int i = 0; i < mpGrid->getNumCandy(); i++)
		{
			int value;
			mapData.read((char*)&value, sizeof(int));
			mCandySpawns.push_back(value);
		}
		//load player if it exists
		if (mpGrid->hasPlayer())
		{
			mapData.read((char*)&mPlayerSpawn, sizeof(int));
		}
	}

	delete mpGridVisualizer;
	mpGridVisualizer = new GridVisualizer(mpGrid);
 	mapData.close();
}

void Map::setLinks(PortalInfo* pPortals)
{
	mLinks.clear();
	PortalInfo portalInfo;
	for (size_t i = 0; i < MAX_PORTALS; i++)
	{
		portalInfo = pPortals[i];

		if (portalInfo.map1ID == mID) //if it matches, add the other id
		{
			PortalLinks link;
			link.portalID = portalInfo.portalID;
			link.portalIndex = portalInfo.portalIndexMap1;
			link.map2ID = portalInfo.map2ID;
			mLinks.push_back(link);
		}

		if (portalInfo.map2ID == mID) //checks other link
		{
			PortalLinks link;
			link.portalID = portalInfo.portalID;
			link.portalIndex = portalInfo.portalIndexMap2;
			link.map2ID = portalInfo.map1ID;
			mLinks.push_back(link);
		}
	}
}

void Map::addGrue(int x, int y)
{
	int gridValue = mpGrid->getSquareIndexFromPixelXY(x, y);
	for each (int value in mGrueSpawns)
	{
		if (value == gridValue)
		{
			return;
		}
	}
	cout << "Grue Spawn Added at " << gridValue << " on Map " << mID << endl;
	mGrueSpawns.push_back(gridValue);
}

void Map::removeGrue(int x, int y)
{
	int gridValue = mpGrid->getSquareIndexFromPixelXY(x, y);

	for (size_t  i = 0; i <mGrueSpawns.size(); i++)
	{
		if (mGrueSpawns.at(i) == gridValue)
		{
			mGrueSpawns.erase(mGrueSpawns.begin() + i);
			cout << "Grue Spawn removed at " << gridValue << " on Map " << mID << endl;
			return;
		}
	}
}

void Map::addCandy(int x, int y)
{
	int gridValue = mpGrid->getSquareIndexFromPixelXY(x, y);
	for each (int value in mCandySpawns)
	{
		if (value == gridValue)
		{
			return;
		}
	}
	cout << "Candy Spawn Added at " << gridValue << " on Map " << mID << endl;
	mCandySpawns.push_back(gridValue);
}

void Map::removeCandy(int x, int y)
{
	int gridValue = mpGrid->getSquareIndexFromPixelXY(x, y);
	for (size_t i = 0; i <mCandySpawns.size(); i++)
	{
		if (mCandySpawns.at(i) == gridValue)
		{
			mCandySpawns.erase(mCandySpawns.begin() + i);
			cout << "Candy Spawn removed at " << gridValue << " on Map " << mID << endl;
			return;
		}
	}
}