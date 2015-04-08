#ifndef UNITMANAGER_H
#define UNITMANAGER_H

#include "KinematicUnit.h"
#include "Trackable.h"
#include <Vector2D.h>
#include "GraphicsBuffer.h"
#include <vector>
using namespace std;

class UnitManager : public Trackable
{
public:
	UnitManager();
	~UnitManager();

	int getSize();

	KinematicUnit* getUnit(int unit);
	void addUnit(KinematicUnit* unit);
	void removeUnit(int unit);

	void init();
	void update(float time);

	void draw(GraphicsBuffer* buffer);

	vector<KinematicUnit*> getUnitsInRadius(Vector2D position, int radius);

private:
	vector<KinematicUnit*> mUnits;
};

#endif