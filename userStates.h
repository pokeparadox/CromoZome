#ifndef STATES_H_INCLUDED
#define STATES_H_INCLUDED

//  Modify this file to tailor the states needed by this game
//  NOTE: Please do not remove the STATE_NULL unless you have good reason to!
//  STATE_TITLE is considered to be the default state it is advised to leave this also
//  #include all state files which correspond to these modes

//	GLOBAL DEFINED FOR ALL STATES

#include "BaseState.h"
#include "StateTitle.h"
#include "StateMain.h"
#include "StateCredits.h"
#include "StateGameover.h"

//STATE_NULL     //  Exit the program, this is declared in the basestate

enum StateModes
{
    STATE_TITLE = 2,    //  Display opening sequences
    STATE_MENU,         //  Display menu options
    STATE_MAIN,         //  Main game sequences
    //STATE_DEMO,         //  Demo mode
    STATE_CREDITS,      //  Display staff credit roll
    STATE_GAMEOVER
};

#endif // STATES_H_INCLUDED
