#pragma once

#include "Trackable.h"

enum MessageType
{
	INVALID_MESSAGE_TYPE = -1,
	PLAYER_MOVETO_MESSAGE = 0,
	ADD_DYNAMIC_MOVE = 1,
	ADD_DYNAMIC_SEEK = 2,
	DELETE_UNIT = 3,
	EXIT_MESSAGE = 4,
	ADD_BOID_MESSAGE = 5,
	REMOVE_BOID_MESSAGE = 6
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

