#include <string>
#include <iostream>
#include <conio.h>

#include "DecisionTreeHelpers.h"
#include "IntDecision.h"
#include "MessageAction.h"

#include "Game.h"
#include "StateMachine.h"
#include "TickState.h"
#include "KaboomState.h"
#include "EndGameState.h"
#include "MemoryTracker.h"

using namespace std;

Game* gpGame = NULL;

void main()
{
	//create the game
	gpGame = new Game;

	//create the state machine
	StateMachine* pStateMachine1 = new StateMachine();

	//create the states
	StateMachineState* pState1 = new TickState(0,10);
	StateMachineState* pState2 = new KaboomState(1);
	StateMachineState* pState3 = new EndGameState(2);
	StateMachineState* pState4 = new TickState(3,5);

	//create the transitions
	StateTransition* pTrans1 = new StateTransition( KABOOM_TRANSITION, 1 );
	StateTransition* pTrans2 = new StateTransition( END_GAME_TRANSITION, 2 );

	//add the transitions to the states
	pState1->addTransition( pTrans1 );
	pState2->addTransition( pTrans2 );

	//add the states to the state machine
	pStateMachine1->addState( pState1 );
	pStateMachine1->addState( pState2 );
	pStateMachine1->addState( pState3 );

	//set the initial state
	pStateMachine1->setInitialStateID(0);

	//add the states to the state machine
	StateMachine* pStateMachine2 = new StateMachine();
	pStateMachine2->addState( pState4 );
	pStateMachine2->addState( pState2 );
	pStateMachine2->addState( pState3 );

	//set the initial state
	pStateMachine2->setInitialStateID(3);

	bool exitting = false;
	bool machineRunning = false;

	while( !exitting )
	{
		if( !machineRunning )
		{
			cout << "\npress any key to start the countdown!";
		}

		if( _kbhit() )
		{
			machineRunning = true;
		}

		if( machineRunning )
		{
			cout << "SM1:";
			pStateMachine1->update();
			
			cout << "SM2:";
			pStateMachine2->update();
		}

		exitting = gpGame->update();
	}

	delete pStateMachine1;
	delete pStateMachine2;
	delete pState1;
	delete pState2;
	delete pState3;
	delete pState4;
	delete pTrans1;
	delete pTrans2;
	delete gpGame;
	gpGame = NULL;

	gMemoryTracker.reportAllocations( cout );

	system("pause");

	//DecisionTree stuff!!!
	//create a bunch of nodes
	IntDecision* pDecision0 = new IntDecision( 0, 10 ); 
	IntDecision* pDecision1 = new IntDecision( 1, 20 ); 
	IntDecision* pDecision2 = new IntDecision( 2, 30 ); 
	MessageAction* pAction0 = new MessageAction( 3, "pAction0" );
	MessageAction* pAction1 = new MessageAction( 4, "pAction1" );
	MessageAction* pAction2 = new MessageAction( 5, "pAction2" );
	MessageAction* pAction3 = new MessageAction( 6, "pAction3" );

	//add nodes to tree
	Decision* start = pDecision0;
	start->setTrueBranch( pAction0 );
	start->setFalseBranch( pDecision1 );
	pDecision1->setTrueBranch( pAction1 );
	pDecision1->setFalseBranch( pDecision2 );
	pDecision2->setTrueBranch( pAction2 );
	pDecision2->setFalseBranch( pAction3);

	pDecision0->setTestValue( 30 );
	pDecision1->setTestValue( 30 );
	pDecision2->setTestValue( 30 );

	DecisionTreeNode* pDecision = start->makeDecision();
	BaseAction* pAction = dynamic_cast<BaseAction*>(pDecision);
	pAction->doAction();

	delete pDecision0;
	delete pDecision1;
	delete pDecision2;
	delete pAction0;
	delete pAction1;
	delete pAction2;
	delete pAction3;

	delete gpGame;
	gMemoryTracker.reportAllocations( cout );

	system("pause");

}