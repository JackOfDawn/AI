#include "AddBoidMessage.h"
#include "Game.h"
#include "Boid.h"
#include "SpriteManager.h"
#include "WeightedBlendingSteering.h"
#include "VelocityMatching.h"
#include "SeparationSteering.h"
#include "CohesionSteering.h"
#include "GroupAlignmentSteering.h"

AddBoidMessage::AddBoidMessage()
	: GameMessage(ADD_DYNAMIC_MOVE)
{
}

AddBoidMessage::~AddBoidMessage()
{
}

void AddBoidMessage::process()
{
	Vector2D position = Vector2D(rand() % gpGame->GetWidth(), rand() % gpGame->GetHeight());
	float orientation = (rand() % 100) / 100.0;
	Vector2D velocity = Vector2D(rand() % 50, rand() % 50);

	// create the unit
	Boid* unit = new Boid(gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID), position, orientation, Vector2D(0, 0), 0, 50, 50);
	
	// steerings and weights for the blending
	vector<Steering*> steerings = vector<Steering*>();
	vector<float> weights = vector<float>();

	// add the steerings to the vector
	steerings.push_back(new VelocityMatching(unit, velocity, .5f, 50));
	steerings.push_back(new SeparationSteering(unit, 150, 2, 50));
	steerings.push_back(new CohesionSteering(unit, 250, -2, 50));
	steerings.push_back(new GroupAlignmentSteering(unit, 500));

	// add the weights to the vector
	weights.push_back(1.1f);
	weights.push_back(1.6f);
	weights.push_back(1.3f);
	weights.push_back(.8f);

	// set the steering of the unit
	unit->setSteering(new WeightedBlendingSteering(steerings, weights, 50));

	// add unit to the unit manager
	gpGame->getUnitManager()->addUnit(unit);
}