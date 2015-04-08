#pragma once

#include "GameMessage.h"
#include <vector>
#include <Vector2D.h>

class GiveUnitPathMessage : public GameMessage
{
public:
	GiveUnitPathMessage(std::vector<Vector2D> path, bool reversed);
	~GiveUnitPathMessage(){};

	void process();
private:
	std::vector<Vector2D> mPath;
	bool mReversed;
};