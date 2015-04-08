#pragma once

#include <Trackable.h>

enum MessageType
{
	INVALID_MESSAGE_TYPE = -1,
	SAVE_MESSAGE,
	LOAD_MESSAGE,
	REMOVE_BLOCK_MESSAGE,
	ADD_BLOCK_MESSAGE,
	CREATE_NEW_MAP_MESSAGE,
	REMOVE_MAP_MESSAGE,
	CHANGE_MAP_MESSAGE,
	TOGGLE_HELP_MESSAGE,
	CHANGE_BLOCK_TYPE
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


