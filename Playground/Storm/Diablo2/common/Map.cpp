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
	mID = INVALID_NUM;
	mLinks = std::vector<PortalLinks>();
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
	}
	mapData.close();

}

void Map::loadGrid(std::ifstream& theStream)
{
	mLinks.clear();
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
		for (size_t i = 0; i < mpGrid->getNumPortals(); i++)
		{
			PortalLinks link;
			mapData.read((char*)&link.portalID, sizeof(int));
			mapData.read((char*)&link.map2ID, sizeof(int));
			mapData.read((char*)&link.portalIndex, sizeof(int));
			mLinks.push_back(link);
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