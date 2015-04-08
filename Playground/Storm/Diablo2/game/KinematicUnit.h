#pragma once

#include "Kinematic.h"
#include "Steering.h"
#include <vector>

/*KinematicUnit - a unit that is derived from the Kinematic class.  Adds behaviors and max speeds and a current Steering.

Dean Lawson
Champlain College
2011

edited by JACK STORM 2014
added an enum for the different steering types
*/

//forward declarations
class Sprite;
class GraphicsBuffer;

extern Steering gNullSteering;//global object - can point to it for a "NULL" Steering

//minmimum forward speed a unit has to have inorder to rotate 
//(keeps unit from spinning in place after stopping
const float MIN_VELOCITY_TO_TURN_SQUARED = 1.0f;

enum SteeringType
{
	NULL_STEERING = -1,
	SEEK = 0,
	ARRIVE,
	WANDER,
	DYNAMIC_SEEK,
	DYNAMIC_FLEE,
	DYNAMIC_ARRIVE,
	NUMBER_OF_STEERING_TYPES
};

class KinematicUnit: public Kinematic
{
public:
	KinematicUnit( Sprite* pSprite, const Vector2D& position, float orientation, const Vector2D& velocity, float rotationVel, float maxVelocity = 1.0f, float maxAcceleration = 1.0f );
	~KinematicUnit();

	//getters and setters
	void setTarget( const Vector2D& target ) { mTarget = target; };
	const Vector2D& getPosition() const { return mPosition; };
	float getMaxVelocity() const { return mMaxVelocity; };
	Vector2D getVelocity() const { return mVelocity; };
	float getMaxAcceleration() const { return mMaxAcceleration; };
	void setVelocity( const Vector2D& velocity ){ mVelocity = velocity; };
	//bool setDynamicSteering(SteeringType behavior, KinematicUnit* pTarget);

	virtual void setNewOrientation();//face the direction you are moving

	//draw yourself to the indicated buffer
	void draw( GraphicsBuffer* pBuffer );
	//move according to the current velocities and update velocities based on current Steering
	void update(float time);

	//initiate behaviors
	void dynamicSeek(std::vector<Vector2D> pTarget, bool reverse);


	void notifyPathEnd();

private:
	Sprite* mpSprite;
	Steering* mpCurrentSteering;
	Vector2D mTarget;//used only for Kinematic seek and arrive
	std::vector<Vector2D> mPathTargets;
	int mCurrentTargetIndex;
	bool mReverse;
	float mMaxVelocity;
	float mMaxAcceleration;

	void setSteering( Steering* pSteering );

};