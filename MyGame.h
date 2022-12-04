#ifndef MYGAME_H
#define MYGAME_H
#include "Engine.h"
/*
PROJECT:	CromoZome
This is the customised game engine class, all overridden code is written here and differs depending on your game.
*/


class MyGame : public Engine
{
	public:
		MyGame();

		virtual PENJIN_ERRORS init();
		virtual void stateManagement();
    private:
        string customControlMap;
};


#endif	//	MYGAME_H
