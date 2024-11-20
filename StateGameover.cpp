#include "StateGameover.h"

StateGameover::StateGameover()
{

}

StateGameover::~StateGameover()
{
	clear();
}

void StateGameover::clear()
{
	// Clear score data from variable cache
    variables[1].setInt(0);
}
void StateGameover::init()
{
    SDL_Rect bounds;
	bounds.x = bounds.y = 0;
	bounds.w = GFX::getXResolution();
	bounds.h = GFX::getYResolution();
	scoreScreen.setBoundaries(bounds);
	scoreScreen.setScoreFile("scores.scr");
	scoreScreen.setTableTitle("Hi-Scores");
	scoreScreen.loadFont("font/origami.ttf", 16);
	scoreScreen.setFontColour(Colour(WHITE));
	scoreScreen.setNameEntry(true);
    const static uint SCORE = 1;
	//	The game over state is normally triggered by death in the main game
	//	Because of this we should check the variable cache for the player details
	if(variables.size()>SCORE)
	{
		if(variables[SCORE].hasInt())
		    if(variables[SCORE].getInt() != 0)
                scoreScreen.setScore(variables[SCORE].getInt());
	}
	scoreScreen.init();
	if(scoreScreen.inNameEntryScreen())
	{
	    /// Currently this track segfaults Pandora and Dingoo builds.
        //music.loadMusic("music/bubbles.xm");
        //music.setLooping(true);
        //music.play()
	}
	GFX::setClearColour(BLACK);
	accumBuffer.update();
}

void StateGameover::userInput()
{
	//	Refresh inputs
	input->update();
#ifdef PLATFORM_PC
    if(input->isQuit())
        nullifyState();
#endif

	//	Check for new score to go into name entry controls
    if(input->isUp())
        scoreScreen.charUp();
    else if(input->isDown())
        scoreScreen.charDown();

#ifdef PLATFORM_WII
    if(input->isTwo())
#else
    if(input->isB())
#endif
        scoreScreen.charInput();
#ifdef PLATFORM_WII
    else if(input->isOne())
#else
    else if(input->isX())
    #endif
        scoreScreen.charDelete();

    //	Check to return to main menu
    #ifndef PLATFORM_WII
    if((input->isB() || input->isStart()) && !scoreScreen.inNameEntryScreen())
    #else
    if((input->isTwo() || input->isPlus())&& !scoreScreen.inNameEntryScreen())
    #endif
    {
        variables.resize(3);
        variables[2].setInt(0);
        setNextState(STATE_TITLE);
    }
    input->resetKeys();
}

void StateGameover::render(SDL_Surface *screen)
{
    /// START BLUR
    scoreScreen.render();
    accumBuffer.update();
    accumBuffer.setAlpha((uchar)196);

    GFX::clearScreen();
    /// Normal render stuff here.

    accumBuffer.render();
    /// END BLUR
}

void StateGameover::update()
{
    scoreScreen.update();
}
