#pragma once

#include <Trackable.h>

enum MessageType
{
	INVALID_MESSAGE_TYPE = -1,
	PLAYER_MOVETO_MESSAGE = 0,
	PATH_TO_MESSAGE = 1,
	CHANGE_ALGORITHM_MESSAGE,
	SHOW_VISITED_NODES_MESSAGE,
	CHANGE_MAP_MESSAGE,
	GIVE_UNIT_PATH_MESSAGE,
	CHECK_PATH_END_MESSAGE,
	SWITCH_DIRECTION_MESSAGE,
	TOGGLE_HELP_MESSAGE
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


