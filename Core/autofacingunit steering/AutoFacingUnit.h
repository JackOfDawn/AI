#pragma once

#include "Kinematic.h"
#include "KinematicUnit.h"
#include "Steering.h"

/*AutoFacingUnit - a kinematic unit which will always face in the direction of it's movement.

Dean Lawson
Champlain College
2011
*/

//forward declaration
class FaceMovementDirectionSteering;

class AutoFacingUnit: public KinematicUnit
{
public:
	AutoFacingUnit( Sprite* pSprite, const Vector2D& position, float orientation, const Vector2D& velocity, float rotationVel, float maxVelocity = 1.0f, float maxAcceleration = 1.0f );
	virtual ~AutoFacingUnit();

	virtual void update(float time);

	friend class AutoFacingUnitDebugContent;

private:
	FaceMovementDirectionSteering* mpFaceDirectionSteering;
	virtual void setSteering( Steering* pSteering );

};