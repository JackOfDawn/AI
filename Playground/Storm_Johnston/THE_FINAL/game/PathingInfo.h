#pragma once
#include "Node.h"
#include <vector>

struct PathingInfo
{
	PathingInfo()
	{
		mapIDs = std::vector<int>();
		nodePairs = std::vector < std::pair<Node*, Node*> >();
		currentPath = 0;
	}

	bool isValid()
	{
		if (mapIDs.size() == 0 || nodePairs.size() == 0)
			return false;
		return true;
	}

	std::vector<int> mapIDs;
	std::vector < std::pair<Node*, Node*> >  nodePairs;
	int currentPath;
};
