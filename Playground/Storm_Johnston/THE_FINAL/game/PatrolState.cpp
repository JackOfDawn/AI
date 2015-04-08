#include "State.h"
#include "Grue.h"
#include "MapHolder.h"
#include "Player.h"



void PatrolState::update(Grue& grue, Player& player)
{
	if (!grue.isPathingValid())
	{
		grue.generatePathing(gpMapHolder->generateValidTarget());
	}

	//check square distance from grue to player
	if (grue.getMapID() == player.getMapID())
	{
		if (Vector2D::distanceSq(player.getPosition(), grue.getPosition()) <= AGRO_DIST * AGRO_DIST && !player.isDead())
		{
			grue.resetPathingInfo();
			if (player.getInvincibility() < 0)
				grue.setState(&State::chase);
			else
			{
				grue.setState(&State::flee);
			}
		}
	}
}

void PatrolState::draw(Sprite* sprite, GraphicsBuffer* buffer, Vector2D& position, float orientation, float scale)
{
	static ALLEGRO_COLOR patrolColor = al_map_rgba_f(0, 1, 0, 1);
	sprite->draw(*buffer, position.getX(), position.getY(), orientation, scale, patrolColor);
}





