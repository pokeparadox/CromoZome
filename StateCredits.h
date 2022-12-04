 #ifndef STATECREDITS_H
#define STATECREDITS_H
#include "userStates.h"     // Make sure your state is listed in this include.
/*
Staff Roll/credits
*/
#include "version.h"
#include "Background.h"
#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Music.h"
#include "Text.h"
#include "TextDoc.h"
#include "Encryption.h"
#include "Timer.h"
#include "Random.h"
#include "Colour.h"
#include "GFX.h"
#include "Emitter.h"
#ifndef PLATFORM_WII
    const static int CREDITS_SIZE = 54;
    const static string CREDITS[]=
#else
    int CREDITS_SIZE = 54;
    char* CREDITS[]=
#endif
{
    "CromoZome",
    "[VERSION]",
    "By Pirate Games",
    "",
    "Game Concept:",
    "PokeParadox",
    "",
    "Project Lead:",
    "PokeParadox",
    "",
    "Assistant Coder:",
    "Cameleon",
    "",
    "Graphics:",
    "Sonistar",
    "",
    "Music:",
    "Magnar",
    "stressball",
    "Okeanos / Nuance",
    "",
    "Testing:",
    "PokeParadox",
    "Cameleon",
    "Jackd",
    "pkostrze",
    "wiseguy109",
    "Tripmonkey uk",
    "David Beoulve",
    "",
    "Thanks:",
    "Squidge",
    "A SN",
    "Dr Ian",
    "rokdcasbah",
    "YakumoFuji",
    "Parkydr",
    "Others I forgot...",
    "",
    "Special Thanks:",
    "foxblock -finding random bugs to fix.",
    "Mariela -your love and support.",
    "MarkoeZ -wacky coding ideas.",
    "Gran -miss you so much.",
    "Dingoonity -made me fix this.",
    "",
    "In Loving Memory Of",
    "Sabrina King",
    "",
    "Project Infinity 2007",
    "Penjin Powered",
    "Pirate Games 2010",
    "pirate-games.co.uk",
    ""
};

class StateCredits : public BaseState
{
    public:
        StateCredits();
        virtual void init();
		virtual void clear();
        virtual void render(SDL_Surface *screen);
        virtual void update();
        virtual void unlimitedUpdate();
        virtual ~StateCredits();
        virtual void userInput();

	private:
        enum NUMBERS
        {
            NUM_AVAS = 16
        };
		Sprite staticAvatars[NUM_AVAS];
		AnimatedSprite camSprite;
		AnimatedSprite mariela;
		uint upOrDown;	//	0 = dwn,1 = up
		bool txtUpOrDown; // true = dwn, false = up
		uint startX,startY,w,h;
		Music music;
		Text text;
		Timer timer;
		uint currentLine;
		Vector2di textPos;
		RandomClass randGen;
		Colour textColour;
		vector <Emitter> emit;

		bool firstRun;
		bool renderParticles;
		int halfRes;
};
#endif	//	STATECREDITS_H

