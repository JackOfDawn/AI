#include "State.h"
#include "MapHolder.h"
#include "Grue.h"
#include "Player.h"


void FleeState::update(Grue& grue, Player& player)
{
	if (!grue.isPathingValid())
	{
		grue.generatePathing(gpMapHolder->generateValidTarget());
	}

	if (player.getInvincibility() < 0)
	{
		grue.resetPathingInfo();

		if (Vector2D::distanceSq(player.getPosition(), grue.getPosition()) > AGRO_DIST * AGRO_DIST)
		{
			//grue.resetPathingInfo(); //having this commented allows for the grues to follow through maps
			grue.setState(&State::patrol); //It will follow till the ends of the earth
		}
		else if (grue.getMapID() == player.getMapID() && !player.isDead())
		{
			grue.resetPathingInfo(); // GET THE FUCK OUT OF THERE
			grue.setState(&State::chase); //BUT NOT WITH THIS
		}
	}
}
void FleeState::draw(Sprite* sprite, GraphicsBuffer* buffer, Vector2D& position, float orientation, float scale)
{
	static ALLEGRO_COLOR fleeColor = al_map_rgba_f(0, 0, 1, 1);
	sprite->draw(*buffer, position.getX(), position.getY(), orientation, scale, fleeColor);
}