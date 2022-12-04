#include "MyGame.h"

MyGame::MyGame()
{
	//	Default constructor
	Penjin::setApplicationName("CromoZome");
}


PENJIN_ERRORS MyGame::init()
{
    GFX::setResolution(320,240);
    setInitialState(STATE_TITLE);
	return GFX::resetScreen();
}

void MyGame::stateManagement()
{
	//	Check if the state itself wants to change states
	uint next = state->getNextState();
	if(state)
	{
		delete state;
		state = NULL;
	}
	if(next == STATE_BASE)
		state = new BaseState;
    else if (next == STATE_MAIN)
        state = new StateMain;
	else if(next == STATE_TITLE)
		state = new StateTitle;
    else if(next == STATE_GAMEOVER)
        state = new StateGameover;
    else if(next == STATE_CREDITS)
        state = new StateCredits;
    else
    {
        cout << ErrorHandler().getErrorString(PENJIN_UNDEFINED_STATE);
        exit(PENJIN_UNDEFINED_STATE);
    }
}
