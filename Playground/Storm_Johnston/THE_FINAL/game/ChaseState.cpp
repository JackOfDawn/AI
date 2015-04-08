#include "State.h"
#include "Grue.h"
#include "MapHolder.h"
#include "Player.h"


void ChaseState::update(Grue& grue, Player& player)
{
	if (!grue.isPathingValid())
	{
		//generate endpoint based on player
		EndPoint targetPoint;
		targetPoint.index = player.getIndexOnGraph();
		targetPoint.mapID = player.getMapID();
		//generatpathing to player
		grue.generatePathing(targetPoint);

		//Check if still too far
		if (grue.getMapID() == player.getMapID())
		{
			
			if (player.getInvincibility() > 0)
			{
				grue.resetPathingInfo(); // GET THE FUCK OUT OF THERE
				grue.setState(&State::flee); //BUT NOT WITH THIS
			}
			else if (Vector2D::distanceSq(player.getPosition(), grue.getPosition()) > AGRO_DIST * AGRO_DIST || player.isDead())
			{
				//grue.resetPathingInfo(); //having this commented allows for the grues to follow through maps
				grue.setState(&State::patrol); //It will follow till the ends of the earth
			}
		}
		else if (player.isDead())
		{
			grue.setState(&State::patrol);
		}
	}
}

void ChaseState::draw(Sprite* sprite, GraphicsBuffer* buffer, Vector2D& position, float orientation, float scale)
{
	static ALLEGRO_COLOR chaseColor = al_map_rgba_f(1, 0, 0, 1);
	sprite->draw(*buffer, position.getX(), position.getY(), orientation, scale, chaseColor);
}