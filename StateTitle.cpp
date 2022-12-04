#include "StateTitle.h"

StateTitle::StateTitle()
{
    nullify = false;
    choice = -1;
}

StateTitle::~StateTitle()
{

}

void StateTitle::init()
{
    input->resetKeys();
    inSubMenu = false;
    inExtrasMenu = false;
	music.loadMusic("music/forbidden_water.mod");
	music.setLooping(true);
	music.play();

    //  Resize the variables so we can pass info between states
	variables.resize(2);    //  No longer need to store volume in here
                            //  0 = Colour selection
                            //  1 = AI (true/false)  // We will also use the same slot to place a score

	string dir = "images/";
	string ext = ".png";

    //  Setup the frames for the animted background
	bgAnimation.loadFrame(dir+"titlefinalblur0"+ext);
	bgAnimation.loadFrame(dir+"titlefinalblur1"+ext);
	bgAnimation.loadFrame(dir+"titlefinalblur2"+ext);
	bgAnimation.loadFrame(dir+"titlefinalblur3"+ext);
	bgAnimation.loadFrame(dir+"titlefinalblur4"+ext);
	bgAnimation.loadFrame(dir+"titlefinalblur5"+ext);
	bgAnimation.setFrameRate(FIFTEEN_FRAMES);
	bgAnimation.setPulsePlay(true);
	bgAnimation.setLooping(true);
//	bgAnimation.setUseHardware(true);

    playBGAnimation = true;

	//  Setup random numbers for choosing random intervals on timers
	randGen.setLimits(1,10);

	//  Setup random intervals for timers
    blurTimer.start();
    blurTimer.setMode(SECONDS);
    titleTimer.start();
    titleTimer.setMode(SECONDS);
    scopeTimer.start();
    scopeTimer.setMode(SECONDS);

    blurLimit = randGen.nextInt();
    titleLimit = randGen.nextInt();
    scopeLimit = randGen.nextInt();

    //  Setup Microscope image
    microscope[0].loadSprite(dir+"overlayL"+ext);
    microscope[0].setTransparentColour(Colour(MAGENTA));
    microscope[0].setPosition(-20,0);
    microscope[1].loadSprite(dir+"overlayR"+ext);
    microscope[1].setTransparentColour(Colour(MAGENTA));
    microscope[1].setPosition(GFX::getXResolution()-98,0);
    microscopeRevert = 1;

    shouldMoveMicroscope = true;

    penjin.loadSprite(dir+"penjin"+ext);
    penjin.setPosition((GFX::getXResolution()-penjin.getWidth())*0.5f,GFX::getYResolution()-penjin.getHeight());
    setupMainMenu();

	eatSound.loadSound(dir+"blurp"+ext);
	plagueSound.loadSound(dir+"plague"+ext);
    splitSound.loadSound(dir+"bloop"+ext);
}

void StateTitle::userInput()
{
    choice = -1;
    // Perform input handling here
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    if(input->isUp())
    {
        menu.menuUp();
		splitSound.play();
        input->resetKeys();
    }
    if(input->isDown())
    {
        menu.menuDown();
		splitSound.play();
        input->resetKeys();
    }
    #ifdef PLATFORM_WII
    if(input->isTwo())
    #else
    if(input->isB())
    #endif
    {
        eatSound.play();
        choice = menu.getSelection();    // selections start at index 1, due to headers
        if(!inSubMenu && !inExtrasMenu)
        {
            mainMenu();
        }
        else if(inSubMenu)
        {
            cromoZomeMenu();
        }
        else if(inExtrasMenu)
        {
            extraMenu();
        }
        input->resetKeys();
    }

    if(input->isStart())
    {
        eatSound.play();
        if(!inSubMenu)
        {
            choice = START;
            inSubMenu = true;
            setupCromoZomeMenu();
        }
        else
        {
            choice = menu.getSelection();
            cromoZomeMenu();
        }
        input->resetKeys();
    }
    #ifdef PLATFORM_WII
    if(input->isOne())
    #else
    if(input->isX())
    #endif
    {
        plagueSound.play();
        if(inSubMenu)
        {
            inSubMenu = false;
            setupMainMenu();
        }
        else if(inExtrasMenu)
        {
            inExtrasMenu = false;
            setupMainMenu();
        }
        else
        {
            //  Prompt quit?
        }
        input->resetKeys();
    }
	#ifdef PLATFORM_GP2X
        //	adjust volume
        uint volume = music.getVolume();
        if(input->isVolumeUp() && volume < 128)
            music.setVolume(volume+2);
        else if(input->isVolumeDown() && volume > 0)
            music.setVolume(volume-2);
    #endif
}

void StateTitle::render()
{
    //  Drawing commands here
	bgAnimation.render();
    penjin.render();
	menu.render();
	for(int i = 1; i >= 0; --i)
        microscope[i].render();
}

void StateTitle::update()
{
    menu.update();
    moveMicroscope();
    pulseBackground();
    pulseTitle();
}

void StateTitle::pulseBackground()
{
    // If the animation is on frame 0
    if(bgAnimation.getCurrentFrame() == 0)
    {
        playBGAnimation = false;
        //  If the timer has run out
        if(blurTimer.getScaledTicks() >= blurLimit)
        {
            //  set the random delay
            blurLimit = randGen.nextInt();
            blurTimer.start();
            playBGAnimation = true;
        }
    }
    if(playBGAnimation)
    {
        bgAnimation.update();
    }
}

void StateTitle::pulseTitle()
{
        //  If the timer has run out
        if(titleTimer.getScaledTicks() >= titleLimit)
        {
            titleLimit = randGen.nextInt();
            titleTimer.start();
        }
}

void StateTitle::moveMicroscope()
{
    // If the Lmicro is -5 to the left
    if(microscope[0].getX() < -22)
    {
        shouldMoveMicroscope = false;
        //  If the timer has run out
        if(scopeTimer.getScaledTicks() >= scopeLimit)
        {
            shouldMoveMicroscope = true;
            scopeLimit = randGen.nextInt();
            scopeTimer.start(); //  resets timer;
        }
    }
    if(shouldMoveMicroscope)
    {
        // If the Lmicro is -5 to the left
        if(microscope[0].getX() > 0 || microscope[0].getX() < -22)
        {
           microscopeRevert = -microscopeRevert;
        }
        microscope[0].setX(microscope[0].getX() + microscopeRevert);
        microscope[1].setX(microscope[1].getX() + microscopeRevert);
    }
}

void StateTitle::setupMainMenu()
{
    menu.clear();
    menu.setMenuStart(GFX::getXResolution()*0.281690141f,GFX::getYResolution()*0.433f);
    menu.setSelection(1);
    menu.loadFont("font/origami.ttf", 16);
    menu.setTextColour(Colour(WHITE));
    menu.addItem(MENU_STRING_ITEM);
    menu.setIsSelectable(false);
    menu.setMenuItemText("Main Menu");
    //menu.loadImage("images/menu/menu_deselected.png");

    menu.addItem(MENU_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Start");
    //menu.loadImage("images/menu/menu_deselected.png");
    //menu.loadSelectionImage("images/menu/menu_selected.png");

    menu.addItem(MENU_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Demo");
    //menu.loadImage("images/menu/menu_deselected.png");
    //menu.loadSelectionImage("images/menu/menu_selected.png");

    menu.addItem(MENU_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Extras");
   // menu.loadImage("images/menu/menu_deselected.png");
   // menu.loadSelectionImage("images/menu/menu_selected.png");

    menu.addItem(MENU_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Quit");
   // menu.loadImage("images/menu/menu_deselected.png");
   // menu.loadSelectionImage("images/menu/menu_selected.png");
}

void StateTitle::setupCromoZomeMenu()
{
    menu.clear();
    menu.setMenuStart(GFX::getXResolution()*0.281690141f,GFX::getYResolution()*0.333f);
    menu.setTextColour(Colour(WHITE));
    menu.setSelection(0);

    menu.addItem(MENU_ANIM_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Strength");
    menu.loadFrames("images/DeselectedButton.gif",9,2);
    menu.loadSelectionFrames("images/StrengthButton.gif",9,2);

    menu.addItem(MENU_ANIM_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Speed");
    menu.loadFrames("images/DeselectedButton.gif",9,2);
    menu.loadSelectionFrames("images/SpeedButton.gif",9,2);

    menu.addItem(MENU_ANIM_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Poison");
    menu.loadFrames("images/DeselectedButton.gif",9,2);
    menu.loadSelectionFrames("images/PoisonButton.gif",9,2);

    menu.addItem(MENU_ANIM_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Back");
    menu.loadFrames("images/DeselectedButton.gif",9,2);
    menu.loadSelectionFrames("images/SpeedButton.gif",9,2);
}

void StateTitle::setupExtraMenu()
{
    menu.clear();
    menu.setMenuStart(GFX::getXResolution()*0.281690141f,GFX::getYResolution()*0.433f);
    menu.setTextColour(Colour(WHITE));
    menu.setSelection(1);

    menu.addItem(MENU_STRING_ITEM);
    menu.setIsSelectable(false);
    menu.setMenuItemText("Extras");

    menu.addItem(MENU_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Hi-Scores");
    //menu.loadImage("images/menu/menu_deselected.png");
    //menu.loadSelectionImage("images/menu/menu_selected.png");

    menu.addItem(MENU_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Credits");
    //menu.loadImage("images/menu/menu_deselected.png");
    //menu.loadSelectionImage("images/menu/menu_selected.png");

    menu.addItem(MENU_STRING_ITEM);
    menu.setIsSelectable(true);
    menu.setMenuItemText("Back");
    //menu.loadImage("images/menu/menu_deselected.png");
    //menu.loadSelectionImage("images/menu/menu_selected.png");

}

void StateTitle::cromoZomeMenu()
{
    if(choice == 0)
    {
        variables[0].setInt(RED_NODE);//	Start as RED
        variables[1].setInt(0);
        setNextState(STATE_MAIN);
    }
    else if (choice == 1)
    {
        variables[0].setInt(BLUE_NODE);//	Start as BLUE
        variables[1].setInt(0);
        setNextState(STATE_MAIN);
    }
    else if (choice == 2)
    {
        variables[0].setInt(GREEN_NODE);   //	Start as GREEN
        variables[1].setInt(0);                 //  Don't run AI player 1
        setNextState(STATE_MAIN);
    }
    else if (choice == 3)
    {
        inSubMenu = false;
        setupMainMenu();
    }
}

void StateTitle::mainMenu()
{
    if (choice == START)
    {
        //  Toggle CromoZome selection
        inSubMenu = true;
        //  setup menu
        setupCromoZomeMenu();
    }
    else if (choice == DEMO)
    {
 		variables[0].setInt(RED_NODE);
		variables[1].setInt(1);         // Activates demo mode
		setNextState(STATE_MAIN);
    }
    else if (choice == EXTRAS)
    {
        inExtrasMenu = true;
        setupExtraMenu();
    }
    else if (choice == QUIT)
    {
        nullifyState();
    }
}

void StateTitle::extraMenu()
{
    if (choice == SCORES)
    {
        variables.resize(1);            //  We resize to stop name entry
		setNextState(STATE_GAMEOVER);
    }
    else if (choice == CREDITS)
    {
        setNextState(STATE_CREDITS);
    }
    else if (choice == BACK)
    {
        inExtrasMenu = false;
        setupMainMenu();
    }
}
