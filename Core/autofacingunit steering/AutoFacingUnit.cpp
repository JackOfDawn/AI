#include "Game.h"
#include "Kinematic.h"
#include "KinematicUnit.h"
#include "AutoFacingUnit.h"
#include "Sprite.h"
#include "GraphicsSystem.h"
#include "Steering.h"
#include "KinematicSeekSteering.h"
#include "KinematicArriveSteering.h"
#include "KinematicWanderSteering.h"
#include "DynamicSeekSteering.h"
#include "DynamicArriveSteering.h"
#include "AlignSteering.h"
#include "FaceMovementDirectionSteering.h"

using namespace std;

AutoFacingUnit::AutoFacingUnit(Sprite *pSprite, const Vector2D &position, float orientation, const Vector2D &velocity, float rotationVel, float maxVelocity, float maxAcceleration)
:KinematicUnit(pSprite, position, orientation, velocity, rotationVel, maxVelocity, maxAcceleration)
,mpFaceDirectionSteering(NULL)
{
	mpFaceDirectionSteering = new FaceMovementDirectionSteering( this );
}

AutoFacingUnit::~AutoFacingUnit()
{
	delete mpFaceDirectionSteering;
}

void AutoFacingUnit::update(float time)
{
	Steering* steering;
	if( mpFaceDirectionSteering != NULL )
	{
		steering = mpFaceDirectionSteering->getSteering();
	}
	else
	{
		steering = &gNullSteering;
	}

	if( steering->shouldApplyAngularDirectly() )
	{
		setRotationalVelocity( steering->getAngular() );

		//since we are applying the steering directly we don't want any rotational velocity
		steering->setAngular( 0.0f );
	}

	KinematicUnit::update( time );
}

//private - always set ApplyAngularDirectly to true
void AutoFacingUnit::setSteering( Steering* pSteering )
{
	//delegate to superclass do do actual setting
	KinematicUnit::setSteering(pSteering);

	//override the ApplyAngularDirectly to true
	mpCurrentSteering->setApplyAngularDirectly( false );
}

