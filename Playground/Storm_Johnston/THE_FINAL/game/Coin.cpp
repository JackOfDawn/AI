#include "Coin.h"
#include "Vector2D.h"
#include <Sprite.h>

Coin::Coin(Vector2D location, Sprite* pSprite)
: KinematicUnit(pSprite, location, 0.01f, Vector2D(), 0.0f)
{
	mValue = 1;
	mType = COIN;
}

Coin::~Coin()
{
}

void Coin::livingDraw(GraphicsBuffer* pBuffer)
{
	mpSprite->draw(*pBuffer, mPosition.getX(), mPosition.getY(), mOrientation, 1);
}
