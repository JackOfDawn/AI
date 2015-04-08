#include "Game.h"
#include <string>
#include "PortalInformation.h"
#include <vector>

class Grid;
class GridVisualizer;

const int INVALID_NUM = -1;



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
	std::vector<PortalLinks> getLinks() { return mLinks; };
private:


	Grid* mpGrid;
	GridVisualizer* mpGridVisualizer;
	std::string mMapFile;
	int mID;
	

	std::vector<PortalLinks> mLinks;
	
};

