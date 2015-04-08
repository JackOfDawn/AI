#pragma once
#include "GameMessage.h"

class Vector2D;

class DisplayMouseCoordMessage : public GameMessage
{
public:
	DisplayMouseCoordMessage(Vector2D &pos);
	~DisplayMouseCoordMessage();

	void process();
private:
	Vector2D mPos;
};