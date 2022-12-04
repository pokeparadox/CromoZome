#ifndef STATETITLE_H_INCLUDED
#define STATETITLE_H_INCLUDED

#include "PenjinStates.h"     // Make sure your state is listed in this include.
/*
Title screen state to show title screen demo
This can be considered as an example of how to create a new state
*/

#include "Animation.h"
#include "Background.h"
#include "Sprite.h"
#include "Music.h"
#include "Random.h"
#include "Sound.h"
#include "Menu.h"

class StateTitle : public BaseState
{
    public:
        StateTitle();
        virtual void init();
        virtual void userInput();
        virtual void render();
        virtual void update();
        //virtual void unlimitedUpdate();
        virtual ~StateTitle();


	private:
        void pulseBackground();
        void pulseTitle();
        void moveMicroscope();

        void setupMainMenu();
        void setupCromoZomeMenu();
        void setupExtraMenu();
        void cromoZomeMenu();
        void mainMenu();
        void extraMenu();

		enum MAIN_MENU_SELECTIONS
		{
			START = 1,
			DEMO,
			EXTRAS,
			QUIT,
		};
		enum EXTRAS_MENU_SELECTIONS
		{
            SCORES = 1,
            CREDITS,
            BACK
		};

		enum NUMBER
		{
		    NUM_MICROSCOPE = 2,
        };

		vector <Sprite> titles;
		vector <bool> titleActive;
		Sprite microscope[NUM_MICROSCOPE];
		bool shouldMoveMicroscope;  //  toggle motion of the Microscope
		int microscopeRevert;       //  1 or -1 used to move the Microscope
		Animation bgAnimation;
        bool playBGAnimation;
		Timer blurTimer;        //  timer for the bgAnimation
		Timer titleTimer;       //  timer for the overlay parts
		Timer scopeTimer;       //  timer for the microscope effect
		RandomClass randGen;
        int blurLimit;
        int titleLimit;
        int scopeLimit;

		Music music;
		Sound eatSound;
		Sound plagueSound;
		Sound splitSound;
		Menu menu;
		bool inSubMenu;
		bool inExtrasMenu;
		int choice;
		Sprite penjin;
};

#endif // STATETITLE_H_INCLUDED
