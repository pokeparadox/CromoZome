#ifndef STATEGAMEOVER_H
#define STATEGAMEOVER_H

#include "userStates.h"     // Make sure your state is listen in this include.

#include "ScoreScreen.h"
#include "Text.h"
#include "Sound.h"
#include "BackBuffer.h"
#include "GFX.h"

class StateGameover : public BaseState
{
    public:
        StateGameover();
        virtual void init();
        virtual void clear();
        virtual void render(SDL_Surface *screen);
        virtual void update();
        virtual void userInput();
        virtual ~StateGameover();

	private:
        Sound eatSound;
		Sound plagueSound;
		Music music;
		ScoreScreen scoreScreen;
		Text text;
		RandomClass randGen;
        BackBuffer accumBuffer;
};
#endif	//	STATEGAMEOVER_H
