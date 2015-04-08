#pragma once

#include "Trackable.h"
#include "Defines.h"

enum MessageType
{
	INVALID_MESSAGE_TYPE = -1,
	PLAYER_MOVETO_MESSAGE = 0,
	END_GAME_MESSAGE = 1,
	DISPLAY_MOUSE_COORD_MESSAGE = 2,
	ADD_DYNAMIC_ARRIVE_MESSAGE = 3,
	ADD_DYNAMIC_SEEK_MESSAGE = 4,
	DELETE_AI_MESSAGE = 5,
	ADD_WANDER_SEEK = 6,
	ADD_WANDER_FLEE = 7
};

class GameMessage: public Trackable
{
public:
	friend class GameMessageManager;

	GameMessage( MessageType type  );
	~GameMessage();

	double getSubmittedTime() const { return mSubmittedTime; };
	double getScheduledTime() const { return mScheduledTime; };

protected:
	MessageType mType;
	double mSubmittedTime;
	double mScheduledTime;

private:
	virtual void process() = 0; 
};

